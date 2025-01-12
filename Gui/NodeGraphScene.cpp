#include "NodeGraphScene.h"

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#else
#include <GL/glew.h>
#endif

#include "Core/NodeGraph.h"
#include "Core/SwwMacro.h"
#include "Core/Plug.h"

#include "Gui/Connector.h"
#include "Gui/PlugItem.h"
#include "Gui/NodeItem.h"

#include "Core/Node.h"
#include "Core/Plug.h"
#include "Gui/ProgressWidget.h"

#include <QVarLengthArray>
#include <QtConcurrentRun>

#include "Gui/GuiGlobal.h"
#include "OpenGL/GLGlobal.h"
#include <QGLContext>

#include "Core/RootNode.h"

#include "Gui/NodeGraphWidget.h"
#include "Gui/Command.h"
#include "Knob/CommandStack.h"

#include <QApplication>
#include <QClipboard>
#include <QMutexLocker>

namespace  SwwGui{

NodeGraphScene::NodeGraphScene(Sww::Core::Node *node, QObject *parent):
    _node(node)/*,_parentNgs(parentNgs)*/,QGraphicsScene(parent),_nodeItemIsMoved(false),_moveCommand(0),_selectCommand(0)
  ,_waitForPushSelectCommand(false),_selectionChanged(false),_waitForPushMoveCommand(false)
{
    reset();
    QObject::connect(node, SIGNAL(emitGraphCleared())
                     ,this, SLOT(reset()));
    QObject::connect(node, SIGNAL(deselectChildren())
                     ,this, SLOT(clearSelection()));
    QObject::connect(this, SIGNAL(selectionChanged())
                     ,this, SLOT(setSelectionChangeFlag()));

    //Sww::Core::JsonSerializer js;
    //std::vector<Sww::Core::Node*> dummy;
    //_currentSelection = js.makeCommand(dummy,Sww::Core::JsonSerializer::kDeselect);


}

NodeGraphScene::~NodeGraphScene(){
    if(_moveCommand) delete _moveCommand;
    if(_selectCommand) delete _selectCommand;

    SWW_DEBUG("delete node graph scene.");
}

void NodeGraphScene::reset(){
    clear();
    _nodeNamesContainer.clear();

    _selectedNodeItems.clear();
    _selectedConnectors.clear();

    _rootItem = new QGraphicsTextItem(0);
    addItem(_rootItem);

}

void NodeGraphScene::addNodeGraphItem(QGraphicsItem *item)
{
    item->setParentItem(_rootItem);
}

bool NodeGraphScene::connect(PlugItem *startPlug, PlugItem *endPlug)
{
    bool stat = _node->connect(startPlug->plug(),endPlug->plug());

    //emit autoUpdateFinished();

    if(!stat)return false;

    //接続変更をemit
    startPlug->emitChangeConnectState(true);
    endPlug->emitChangeConnectState(true);
    connectGui(startPlug,endPlug);
    return true;
}

bool NodeGraphScene::connect(const QString &nodeNameStart, const QString &plugNameStart,
                             const QString &nodeNameEnd, const QString &plugNameEnd)
{
    NodeItem* nStart;
    NodeItem* nEnd;
    PlugItem* pStart;
    PlugItem* pEnd;

    //SWW_DEBUG_VALUE(_nodeGraph);
    //_nodeGraph->printAllNodes();

    SWW_CHECK_BOOL(getNodeItem(nodeNameStart,nStart));
    SWW_CHECK_BOOL(getNodeItem(nodeNameEnd,nEnd));
    SWW_CHECK_BOOL(nStart->getPlugItem(plugNameStart,pStart));
    SWW_CHECK_BOOL(nEnd->getPlugItem(plugNameEnd,pEnd));

    if(!pStart->plug()->isConnectable())
    {
        SWW_ERR("plug not connectable:" << pStart->plug()->getName()  << nStart->node()->getName());
        return false;
    }
    if(!pEnd->plug()->isConnectable())
    {
        SWW_ERR("plug not connectable:" << pEnd->plug()->getName()  << nEnd->node()->getName());
        return false;
    }

    SWW_CHECK_BOOL(connect(pStart,pEnd));

    return true;
}

void NodeGraphScene::connectGui(PlugItem *startPlug, PlugItem *endPlug)
{
    //input->inputの接続の場合、passthroughに
    if(startPlug->_plug->isPassthrough() && !startPlug->_plug->isOutput() && !startPlug->isOutput()){
        startPlug = startPlug->getplugItemPassthrough();
        Q_ASSERT_X(startPlug,startPlug->_plug->getDebugName().c_str(),"get passthrough plug failed.");
    }

    Connector *newConnector = new Connector(startPlug,endPlug);

    //ノードを移動したときに再描画するために、NodeItemにConnecttorを渡しておく
    startPlug->parentNodeItem()->addConnector(newConnector);
    endPlug->parentNodeItem()->addConnector(newConnector);

    startPlug->connector(newConnector);
    endPlug->connector(newConnector);

    newConnector->setZValue(-1000);

    addNodeGraphItem(newConnector);
    newConnector->updateLine();
}

bool NodeGraphScene::connectInternal(PlugItem *startPlug, PlugItem *endPlug) //p1 = output, p2 = input
{
    return _node->connect(startPlug->plug(),endPlug->plug());
}

bool NodeGraphScene::disconnect(Connector *c)
{
    bool stat = disconnectInternal(c);
    if(!stat) return false;
    disconnectGui(c);

    return true;
}

void NodeGraphScene::disconnectGui(Connector* c,bool withEmit)
{
    c->startPlug()->parentNodeItem()->removeConnector(c);
    c->endPlug()->parentNodeItem()->removeConnector(c);
    c->endPlug()->connector(0);
    c->startPlug()->connector(0);

    c->startPlug()->emitChangeConnectState(false);
    if(withEmit)c->endPlug()->emitChangeConnectState(false);
    removeItem(c);
    c->update();
    delete c;
}

bool NodeGraphScene::disconnectInternal(Connector *c)
{
    return _node->disconnect(c->startPlug()->plug(),c->endPlug()->plug());
}

NodeItem* NodeGraphScene::createNode(const QString &nodeType,
                                     const QPointF &pos,
                                     const QString &nodeName)
{
    bool stat;
    Sww::Core::Node *nodePtr = 0;

    stat = _node->createNode(nodeType.toStdString(),nodePtr,nodeName.toStdString());

    if(!stat)
    {
        SWW_ERR("nodeItem create failed.");
        return 0;
    }

    NodeItem *nodeItem1 = new NodeItem(nodePtr,this);
    QObject::connect(nodePtr,SIGNAL(runSignal(Sww::Core::PlugBase*)),this,SLOT(runBackGround(Sww::Core::PlugBase*)));


    nodeItem1->setPos(pos);
    nodePtr->posInGraph(Imath::V2f(pos.x(),pos.y()));
    addNodeGraphItem(nodeItem1);
    _nodeNamesContainer[nodePtr->getName().c_str()] = nodeItem1;

    GuiGlobal::currentSceneModified();

    return nodeItem1;
}

bool NodeGraphScene::removeNode(NodeItem *n)
{
    SwwOpenGL::GLGlobal::makeCurrent();
    //    n->preRemoveMyself();
    //    _nodeNamesContainer.remove(n->node()->getName().c_str());
    //    _node->removeNode(n->node()->getName());
    //    delete n;

    n->preRemoveMyself();
    std::string nodeName = n->_node->getName();
    delete n;
    _nodeNamesContainer.remove(nodeName.c_str());
    _node->removeNode(nodeName);
    SwwOpenGL::GLGlobal::doneCurrent();
    return true;
}

bool NodeGraphScene::getNodeItem(const QString &nodeName, NodeItem *&nodeItemPtr)
{
    nodeItemPtr = _nodeNamesContainer.value(nodeName);
    if(!nodeItemPtr)
    {
        SWW_ERR("get NodeItem failed. does not exist:" << nodeName.toStdString());
        return false;
    }

    return true;
}

void NodeGraphScene::reconnect(Connector *c,PlugItem* picked,bool changeStart)
{
    bool stat ;

    if(picked->plug()->isFull())
    {
        stat = disconnect(picked->connector());
        if(!stat)
        {
            SWW_ERR("reconnect failed.");
            c->updateLine();
            c->update();
            return;
        }
    }

    stat = disconnectInternal(c);//内部接続解除

    if(!stat)
    {
        SWW_ERR("reconnect failed.");
        c->updateLine();
        c->update();
        return;
    }

    //内部接続
    if(changeStart)stat = connectInternal(picked,c->endPlug());
    else stat = connectInternal(c->startPlug(),picked);

    if(!stat)
    {
        SWW_ERR("reconnect failed.");
        connectInternal(c->startPlug(),c->endPlug());   //つなぎ直す
        c->updateLine();
        c->update();
        return;
    }

    if(changeStart)
    {
        c->startPlug()->emitChangeConnectState(false);

        c->startPlug()->parentNodeItem()->removeConnector(c);   //ノードから登録解除
        c->startPlug()->connector(0);                               //ポートから登録解除
        c->startPlug(picked);     //接続先変更
    }else
    {
        c->endPlug()->emitChangeConnectState(false);

        c->endPlug()->parentNodeItem()->removeConnector(c);   //ノードから登録解除
        c->endPlug()->connector(0);                               //ポートから登録解除
        c->endPlug(picked);     //接続先変更
    }

    picked->parentNodeItem()->addConnector(c);            //ノードに登録
    picked->connector(c);                            //ポートに登録

    c->updateLine();
    c->update();

    c->startPlug()->emitChangeConnectState(true);
    c->endPlug()->emitChangeConnectState(true);

}


void NodeGraphScene::setFrameRange(int start, int end)
{
    _node->_rootNode->startFrame(start);
    _node->_rootNode->endFrame(end);
    if(start > _node->_rootNode->currentFrame())
    {
        _node->_rootNode->setGlobalFrame(start);
    }

    emit _node->_rootNode->emitframeRangeChanged();
}

bool NodeGraphScene::runBackGround(Sww::Core::PlugBase *p)
{
    Sww::Core::InfoToSubThread info;
    info.start = _node->_rootNode->startFrame();
    info.end = _node->_rootNode->endFrame();
    info.current = _node->_rootNode->currentFrame();
    info.node = _node->_rootNode;
    info.plug = p;
    info.stopped = new bool(false);

    ProgressWidget* pWidget = new ProgressWidget(info.stopped);
    pWidget->setInitialStatus(info.start,info.end,info.plug->getDebugName().c_str());
    pWidget->setCurrentStatus(info.start,0);
    pWidget->setAttribute(Qt::WA_DeleteOnClose);
    pWidget->setWindowFlags(Qt::WindowStaysOnTopHint);

    //info.stopped = pWidget->stoppedPtr();

    QObject::connect(_node->_rootNode, SIGNAL(runFinished()), pWidget, SLOT(close()));
    pWidget->show();

#if 1 //２次スレッド
    QtConcurrent::run(Sww::Core::RootNode::run,pWidget,info);
#else
    Sww::Core::RootNode::run(pWidget,info);
#endif

    return true;
}


void NodeGraphScene::setup(){

    reset();

    QMap<std::string, Sww::Core::Node::Ptr>::iterator it = _node->_children.begin();
    while( it != _node->_children.end() )
    {
        Sww::Core::Node::Ptr& node = it.value();
        NodeItem *nodeItem1 = new NodeItem(node.get(),this);
        QObject::connect(node.get(),SIGNAL(runSignal(Sww::Core::PlugBase*)),this,SLOT(runBackGround(Sww::Core::PlugBase*)));

        nodeItem1->setPos(QPointF(node->posInGraph().x,node->posInGraph().y));
        addNodeGraphItem(nodeItem1);
        _nodeNamesContainer[node->getName().c_str()] = nodeItem1;

        ++it;
    }

    //2回目のループ　コネクトする
    it = _node->_children.begin();
    while( it != _node->_children.end() )
    {
        Sww::Core::Node::Ptr& node = it.value();
        NodeItem* nodeItem = _nodeNamesContainer.value(node->getName().c_str());

        Q_FOREACH(PlugItem* pi,nodeItem->_plugItemList){
            if(!pi->isOutput()){
                PlugItem* piIn = pi;

                std::vector<std::string> connectedNames;
                piIn->_plug->getConnPlugNames(connectedNames,true,true);

                for(std::vector<std::string>::iterator ite = connectedNames.begin();ite != connectedNames.end();++ite){
                    std::string& name = *ite;
                    QString nameQt = name.c_str();

                    //インデックス部分 [d] を消去
                    QString pattern = "\\[\\d+\\]$";      //　\\d→数字 $→末尾
                    QRegExp r(pattern);
                    nameQt = nameQt.remove(r);

                    QString connectedNodeName = nameQt.split(".")[0];
                    QString connectedPlugName = nameQt.split(".")[1];

                    if(_nodeNamesContainer.contains(connectedNodeName)){
                        NodeItem* connectedNodeItem = _nodeNamesContainer[connectedNodeName];
                        if(connectedNodeItem->_plugNamesContainer.contains(connectedPlugName)){
                            PlugItem* piOut = connectedNodeItem->_plugNamesContainer[connectedPlugName];
                            connectGui(piOut,piIn);
                        }
                    }

                }//foreach connected names.
            }// if input.
        }//foreach plug item.

        ++it;
    }//foreach nodetable.

    SWW_DEBUG("NodeGraphScene setup.");

}

void NodeGraphScene::add2ConnectHandler(Sww::Core::ConnectHandler *c,int index){
    _node->add2ConnectHandler(c,index);
}

bool NodeGraphScene::connectByHandler(){
    return _node->connectByHandler();
}

const std::string& NodeGraphScene::getNodeName() const{
    Q_ASSERT(_node);
    return _node->getName();
}

QPointF NodeGraphScene::posFromRoot(QPointF scenePos){
    float x = scenePos.x() - _rootItem->x();
    float y = scenePos.y() - _rootItem->y();
    return QPointF(x,y);
}


void NodeGraphScene::deleteSelectedNodes(){
    using namespace Sww::Core;

    QMutexLocker locker(&_mutex);

    if(_selectedNodeItems.empty()){
        Q_FOREACH(Connector* c,_selectedConnectors){
            disconnect(c);
        }
    }else{
        std::vector<Sww::Core::Node*> selectedNodes;
        Q_FOREACH(SwwGui::NodeItem* item,_selectedNodeItems){
            selectedNodes.push_back(item->node());
        }

        SwwGui::RemoveCommand* cmd = new SwwGui::RemoveCommand(selectedNodes,this);
        SwwKnob::CommandStack::pushCommand(cmd);

        Q_FOREACH(NodeItem* n,_selectedNodeItems){
            n->removeMyself();
        }
    }
    _selectedNodeItems.clear();
    _selectedConnectors.clear();
}

void NodeGraphScene::copyToClipBoard(){

    QMutexLocker locker(&_mutex);

    std::vector<Sww::Core::Node*> selectedNodes;
    Q_FOREACH(SwwGui::NodeItem* item,_selectedNodeItems){
        selectedNodes.push_back(item->node());
    }
    Sww::Core::JsonSerializer js;
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(js.serialize(selectedNodes).c_str());

    SWW_DEBUG("copy to clipboard.");
}

void NodeGraphScene::groupingNodes(QPointF mousePos){

    QMutexLocker locker(&_mutex);

    std::vector<Sww::Core::Node*> selectedNodes;
    Q_FOREACH(SwwGui::NodeItem* item,_selectedNodeItems){
        selectedNodes.push_back(item->node());
    }

    Sww::Core::JsonSerializer js;
    std::string jStr = js.serialize(selectedNodes);

    SwwJson j = SwwJson::parse(jStr);
    Sww::Core::JsonDeserializer jd;
    //QPointF p = getMousePos();
    NodeItem* nodeItem = createNode("Null",mousePos,"group");

    try{
        jd.setFlags(Sww::Core::JsonDeserializer::kSolveNameConflict);
        Imath::Box2f dummy;
        if(!jd.deserialize(j,nodeItem->node(),dummy)){
            SWW_ERR("deserialize failed");
        }
    }
    catch(...){
        SWW_ERR("parse failed.");
    }

    SWW_DEBUG("grouping");
}

void NodeGraphScene::storeSelectedNodesAndConnectors(){
    using namespace Sww::Core;

    if(_selectionChanged){
        QMutexLocker locker(&_mutex);

        clearNodesAndConnectorsSelection();

        Q_FOREACH(QGraphicsItem* item,selectedItems()){
            NodeItem* n = qgraphicsitem_cast<NodeItem*>(item);
            if(n){
                _selectedNodeItems.push_back(n);
            }else{
                Connector* c = qgraphicsitem_cast<Connector*>(item);
                if(c) _selectedConnectors.push_back(c);
            }
        }

        _selectionChanged = false;
    }

}

void NodeGraphScene::clearNodesAndConnectorsSelection(){
    _selectedNodeItems.clear();
    _selectedConnectors.clear();
}

void NodeGraphScene::createMod(const std::string& jsonStr,QPointF mousePos ){

    try{
        SwwJson j = SwwJson::parse(jsonStr);

        Sww::Core::JsonDeserializer jd;
        jd.setFlags(Sww::Core::JsonDeserializer::kSolveNameConflict);

        Imath::Box2f pastedArea;
        if(!jd.deserialize(j,node(),pastedArea)){
            SWW_ERR("deserialize failed");
        }

        QPointF nodePosOffset = mousePos - QPointF(pastedArea.min.x,pastedArea.min.y);

        setup();

        //select pasted nodes.
        for (SwwJson::iterator it = j.begin(); it != j.end(); ++it) {
            std::string nodeName = it.key();
            NodeItem* nodeItemPtr;
            if(getNodeItem(nodeName.c_str(),nodeItemPtr)){
                nodeItemPtr->setSelected(true);
                nodeItemPtr->moveBy(nodePosOffset.x(),nodePosOffset.y());
                nodeItemPtr->updateConnectors();
            }

        }//foreach pasted.

    }//try
    catch(...){
        SWW_ERR("parse failed.");
    }
}

void NodeGraphScene::createMod(const std::string& jsonStr){

    try{
        SwwJson j = SwwJson::parse(jsonStr);
        Sww::Core::JsonDeserializer jd;

        Imath::Box2f pastedArea;
        if(!jd.deserialize(j,node(),pastedArea)){
            SWW_ERR("deserialize failed");
        }
        setup();

        //select pasted nodes.
        for (SwwJson::iterator it = j.begin(); it != j.end(); ++it) {
            std::string nodeName = it.key();
            NodeItem* nodeItemPtr;
            if(getNodeItem(nodeName.c_str(),nodeItemPtr)){
                nodeItemPtr->setSelected(true);
                nodeItemPtr->updateConnectors();
            }

        }//foreach pasted.

    }//try
    catch(...){
        SWW_ERR("parse failed.");
    }
}

void NodeGraphScene::selectNodeItems(const std::vector<std::string> &nodeNames){

    QMutexLocker locker(&_mutex);

    _selectedNodeItems.clear();
    _selectedConnectors.clear();

    if(nodeNames.empty()){
        clearSelection();
        update();
        return;
    }
    for(auto nodeName:nodeNames){
        NodeItem* item = _nodeNamesContainer.value(nodeName.c_str());
        item->setSelected(true);
        _selectedNodeItems.push_back(item);
    }

    update();
    Q_EMIT nodeIsSelected();

    _selectionChanged = false;
}

void NodeGraphScene::emitNodeIsSelected(){
    Q_EMIT nodeIsSelected();
}

void NodeGraphScene::prepareMoveCommand(){
    using namespace Sww::Core;

    QMutexLocker locker(&_mutex);

    std::vector<Sww::Core::Node*>nodes;
    Q_FOREACH (NodeItem* n, _selectedNodeItems) {
        nodes.push_back(n->_node);
    }

    if(_moveCommand) delete _moveCommand;
    _moveCommand = new MoveCommand(nodes,this);

    _waitForPushMoveCommand = true;
}

void NodeGraphScene::pushMoveCommand(){

    if(_nodeItemIsMoved && _waitForPushMoveCommand){
        SwwGui::GuiGlobal::currentSceneModified();

        //push move command****************************************
        using namespace Sww::Core;
        QMutexLocker locker(&_mutex);

        std::vector<Sww::Core::Node*>nodes;
        Q_FOREACH (NodeItem* n, _selectedNodeItems) {
            nodes.push_back(n->_node);
        }
        _moveCommand->setAfterPos(nodes);
        SwwKnob::CommandStack::pushCommand(_moveCommand);
        _moveCommand = 0;

        //*********************************************************
        _nodeItemIsMoved = false;
        _waitForPushMoveCommand = false;

    }
}

void NodeGraphScene::prepareSelectCommand(){

    using namespace Sww::Core;

    if(_waitForPushSelectCommand) return;

    QMutexLocker locker(&_mutex);

    std::vector<std::string>nodeNames;
    Q_FOREACH (NodeItem* n, _selectedNodeItems) {
        nodeNames.push_back(n->_node->getName());
    }

    if(_selectCommand) delete _selectCommand;
    _selectCommand = new SelectCommand(nodeNames,this);

    _waitForPushSelectCommand = true;

}

void NodeGraphScene::pushSelectCommand(){

    if(_waitForPushSelectCommand){

        QMutexLocker locker(&_mutex);

        std::vector<std::string>nodeNames;
        Q_FOREACH (NodeItem* n, _selectedNodeItems) {
            nodeNames.push_back(n->_node->getName());
        }

        Q_ASSERT(_selectCommand);
        _selectCommand->setAfterSelect(nodeNames);
        Q_ASSERT(_selectCommand);
        SwwKnob::CommandStack::pushCommand(_selectCommand);
        _selectCommand = 0;

        Q_EMIT nodeIsSelected();

        _waitForPushSelectCommand = false;
    }
}

void NodeGraphScene::setSelectionChangeFlag(){
    _selectionChanged = true;
}

//NodeGraphScene* NodeGraphScene::parentNgs(){
////    Q_ASSERT(0);
////    return this;//dummy


//}

std::string NodeGraphScene::getPath()const{
    Q_ASSERT(_node);
    return _node->getPath();
}

QRectF NodeGraphScene::getSceneRect()const{

    QRectF result;

    if(!_selectedNodeItems.empty()){
        Q_FOREACH (NodeItem* n, _selectedNodeItems) {
            result = result.united(n->sceneBoundingRectWithTitleItem());
        }
    }

    SWW_DEBUG_VALUE(_selectedNodeItems.size());

    return result;

}

}//ns
