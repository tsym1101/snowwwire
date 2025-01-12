#include "NodeItem.h"

#include <QMessageBox>
#include <QGroupBox>
#include <math.h>

#include <QSpacerItem>

//#include "SwwGlobal/SwwGlobal.h"
//#include <Gui/Knobmaker.h>
//#include <Gui/KnobWidget.h>
//#include <Core/Utility.h>

//#include <QVBoxLayout>

#include "Core/Node.h"
#include "Core/PlugBase.h"
#include "Core/NodeGraph.h"
#include "Core/PlugType.h"

#include "Gui/NodeGraphScene.h"

#include "Gui/Connector.h"
#include "Gui/PlugItem.h"

#include "Knob/KnobMaker.h"

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include "Knob/KnobObject.h"
#else
#include <GL/glew.h>
#include "Knob/KnobObject.h"
#endif

#include <QTextCodec>
#include "Gui/NodeGraphWidget.h"
#include "Knob/KnobBaseWidget.h"
#include "Knob/KnobGroupWidget.h"
#include "Gui/AddDynamicPlugWidget.h"
#include "Gui/GuiGlobal.h"
#include "Knob/CommandStack.h"
#include "Gui/Command.h"


using namespace  SwwGui;

//const QString fontFamilyNodeHeader = "Courier";
const QString fontFamilyNodeHeader = "Tahoma";
const QString fontFamilyNodeBody = "Tahoma";

NodeItem::NodeItem(Sww::Core::Node *node , NodeGraphScene *scene, QGraphicsItem* parent):_node(node),QGraphicsItem(parent)
  ,_orgX(-75),_orgY(0),_nodeWidth(150),_nodeHeight(100),_plugNameOrgY(40),_plugNameHeight(25),
    _knobGroup(0),_scene(scene),_childScene(0),_contextMenu(0)/*,_positionChanged(false),_clicked(false),_moveCommand(0)*/
{
    _node = node;
    setFlag(ItemIsMovable);     //移動可能
    setFlag(ItemIsSelectable);
    setCacheMode(DeviceCoordinateCache);    //キャッシュモード
    setFlag((ItemIsFocusable));
    setFlag(ItemSendsGeometryChanges);     //位置変更を監視するかどうか
    setup();

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
#else
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif

    QString tp = _node->toolTip();
    setToolTip(tp);

    QObject::connect(node, SIGNAL(emitAddDynamicPlug(Sww::Core::PlugBase*))
                     ,this, SLOT(addDynamicPlugItem(Sww::Core::PlugBase*)));
    QObject::connect(node, SIGNAL(emitDeleteDynamicPlug(const Sww::Core::PlugMetaPtr&))
                     ,this, SLOT(deleteDynamicPlugItem(const Sww::Core::PlugMetaPtr&)));
    QObject::connect(node, SIGNAL(emitConnectDynamicPlug(const Sww::Core::ConnectState*))
                     ,this, SLOT(connectPlugItem(const Sww::Core::ConnectState*)) );
    QObject::connect(node, SIGNAL(emitErrorOccurred(bool))
                     ,this, SLOT(recieveErrorOccurred(bool)) );
    QObject::connect(node, SIGNAL(nodeStateChanged())
                     ,this, SLOT(updateNodeItem()) );
    QObject::connect(node, SIGNAL(removeMyself())
                     ,this, SLOT(removeMyself()) );
    QObject::connect(node, SIGNAL(selectMyself())
                     ,this, SLOT(selectMyself()) );

}

NodeItem::~NodeItem()
{
    if(_knobGroup)
    {
        if(_knobGroup->parent()){
            _knobGroup->setParent(0);
        }
        delete _knobGroup;
    }

    if(_contextMenu && !_contextMenu->parent()){
        delete _contextMenu;
    }

    //    if(_moveCommand){
    //        delete _moveCommand;
    //    }
}

QWidget* NodeItem::getKnob(){return _knobGroup;}

NodeGraphScene* NodeItem::nodeGraphScene(){
    //return (NodeGraphScene*)scene();
    return _scene;
}

bool NodeItem::getPlugItem(const QString &plugName, PlugItem *&p)
{
    p = _plugNamesContainer[plugName];
    if(!p)
    {
        SWW_ERR("get plugItem failed. does not exist:" << plugName.toStdString());

        return false;
    }

    return true;
}

void NodeItem::preRemoveMyself()
{
    foreach(Connector* cn,_connectorList)
    {
        nodeGraphScene()->disconnectGui(cn);
    }
}

void NodeItem::removeMyself()
{
    nodeGraphScene()->removeNode(this);
}

void NodeItem::selectMyself()
{
    setSelected(true);
}

void NodeItem::addConnector(Connector *connector)
{
    _connectorList << connector;
}

void NodeItem::removeConnector(Connector *cnct)
{
    _connectorList.removeOne(cnct);
}

void NodeItem::showInfo()
{
    _node->printIntenalConnection();
}

void NodeItem::setup()
{
    if(! _node) return;

    //名前を取得
    _myName = _node->getName().c_str();

    _myPen.setStyle(Qt::NoPen);

    if(_node->isRunner()){
        _myPen.setColor(QColor(0,20,80));
        _myPen.setStyle(Qt::SolidLine);
        _myPen.setWidth(5);
    }

    int currentInputPlug = 0;
    int currentOutputPlug = _node->inputConnectablePlugCount();

    //    for(int i = 0;i < _node->inputPlugCount();i++)
    //    {
    //        Q_ASSERT_X(_node->_plugArrayInput.size() > i,_node->getName().c_str(),"out of range.");
    //        Sww::Core::PlugBase::Ptr& plugTmp = _node->_plugArrayInput[i];

    //        setPlugItem(plugTmp.get(),currentInputPlug);
    //        if(plugTmp->isConnectable())
    //        {
    //            currentInputPlug ++ ;
    //        }
    //    }

    //    for(int i = 0;i < _node->outputPlugCount();i++)
    //    {

    //        Q_ASSERT_X(_node->_plugArrayOutput.size() > i,_node->getName().c_str(),"out of range.");
    //        Sww::Core::PlugBase::Ptr& plugTmp = _node->_plugArrayOutput[i];

    //        setPlugItem(plugTmp.get(),currentOutputPlug + currentInputPlug);
    //        if(plugTmp->isConnectable())
    //        {
    //            currentOutputPlug ++ ;
    //        }
    //    }

    int currentPlugDynamic = _node->inputConnectablePlugCount() + _node->outputConnectablePlugCount();
    //int currentOutputPlugDynamic = currentInputPlugDynamic;

    for(Sww::Core::PlugBase::Ptr p : _node->_plugArray){
        if(!p->isDynamic()){
            if(!p->isOutput()){
                setPlugItem(p.get(),currentInputPlug);
                if(p->isConnectable())
                {
                    currentInputPlug ++ ;
                    // currentOutputPlug ++ ;
                }
            }else{
                setPlugItem(p.get(),currentOutputPlug);
                if(p->isConnectable())
                {
                    currentOutputPlug ++ ;
                }
            }

        }
    }

    int totalConnectablePlugCount = _node->inputConnectablePlugCount() + _node->outputConnectablePlugCount();
    _nodeHeight = _plugNameOrgY + _plugNameHeight *  totalConnectablePlugCount;

//    QGraphicsTextItem* textItem = new QGraphicsTextItem(_myName,this);
    _titleItem = new QGraphicsTextItem(_myName,this);

    QFont font;
    font.setBold(true);
    font.setPointSize(20);
    font.setFamily(fontFamilyNodeBody);

    _titleItem->setFont(font);
    _titleItem->setDefaultTextColor(QColor(255,255,255));

    QRectF rect = _titleItem->boundingRect();
    _titleItem->setX(rect.width() *  -0.5);
    _titleItem->setY(rect.height() *  -1.0);

    _staticConnectablePlugItemCount = currentInputPlug + currentOutputPlug;

    createKnobGroup();

    for(Sww::Core::PlugBase::Ptr p : _node->_plugArray){
        if(p->isDynamic()){
            if(!p->isOutput()){
                addDynamicPlugItem(p.get(),currentPlugDynamic);
                currentPlugDynamic++;
            }else{
                addDynamicPlugItem(p.get(),currentPlugDynamic);
                currentPlugDynamic++;
            }
        }
    }

}

PlugItem* NodeItem::setPlugItem(Sww::Core::PlugBase *plugPtr, int current)
{
    if((!plugPtr->isConnectable()) && plugPtr->isHidden()) return 0;

    PlugItem* plugItem = new PlugItem(plugPtr,this,this);
    _plugItemList << plugItem;
    _plugNamesContainer[plugPtr->getName().c_str()] = plugItem;

    if(!plugPtr->isConnectable())
    {
        plugItem->hide();
        return plugItem;
    }

    int side = -1;
    if(plugPtr->isOutput())side = 1;
    float x = (_nodeWidth / 2) * side;
    float y = (current * _plugNameHeight)  + _plugNameOrgY;

    plugItem->setX(x);
    plugItem->setY(y);

    QGraphicsTextItem* textItem = new QGraphicsTextItem(plugPtr->getName().c_str(),plugItem);

    QFont font;
    font.setBold(false);
    font.setFamily(fontFamilyNodeBody);
    font.setPixelSize(20);
    textItem->setFont(font);

    QRectF rect = textItem->boundingRect();
    if(!plugPtr->isOutput())textItem->setX(10);
    else textItem->setX(rect.width() * -1 - 10);
    textItem->setY(rect.height() * -0.6);
    textItem->setDefaultTextColor(QColor(30,30,30));

    return plugItem;
}

QRectF NodeItem::boundingRect() const
{
    return QRectF(_orgX - 3, _orgY - 3,_nodeWidth + 6 ,_nodeHeight + 6);
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
//    painter->setClipRect(option->exposedRect);
    painter->setRenderHint(QPainter::Antialiasing);

    QBrush b;

    if(_node->errorOccurred()){
        b.setColor(Qt::darkRed);
        b.setStyle(Qt::SolidPattern);
    }else{
        b.setColor(Qt::lightGray);
        b.setStyle(Qt::SolidPattern);
    }

    painter->setBrush(b);

    if(isSelected())
        painter->setPen(QPen(QColor(0,80,170),8));
    else{
        painter->setPen(_myPen);
    }

    painter->drawRoundedRect(_orgX, _orgY,_nodeWidth,_nodeHeight,5,5);
}

void NodeItem::changeExcutingColor()
{
    _myPen.setStyle(Qt::SolidLine);
    _myPen.setColor(Qt::yellow);
    _myPen.setWidth(5);

    update();
}

void NodeItem::changeOrdinaryColor()
{
    _myPen.setWidth(5);
    _myPen.setColor(Qt::black);
    update();
}

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

#if 1
    if ( event->type ( ) == QEvent::GraphicsSceneMouseDoubleClick )
    {
        Q_ASSERT(!scene()->views().isEmpty());
        NodeGraphWidget* widget = dynamic_cast<NodeGraphWidget*>(scene()->views()[0]);
        if(widget){
            widget->setSceneWithStackCommand(node()->getPath(),false);
        }

        return;
    }
#endif

    QGraphicsItem::mousePressEvent(event);

}

void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsItem::mouseMoveEvent(event); // 先に呼び出さないとコネクタの描画が遅延する

    if (event->lastPos() != event->pos()){
            QList<QGraphicsItem*> items = scene()->selectedItems();
            foreach (QGraphicsItem* item, items) {
                NodeItem* n = qgraphicsitem_cast< NodeItem*>(item);
                if(n)n->updateConnectors();
            }
    }

    event->ignore();

}

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //    if(_positionChanged){
    //        SwwGui::GuiGlobal::currentSceneModified();
    //        updateConnectors();
    //        //push move command****************************************
    //        using namespace Sww::Core;
    //        std::vector<Sww::Core::Node*>nodes;
    //        QList<QGraphicsItem*> items = scene()->selectedItems();
    //        Q_FOREACH (QGraphicsItem* item, items) {
    //            NodeItem* n = qgraphicsitem_cast< NodeItem*>(item);
    //            if(n)nodes.push_back(n->_node);
    //        }
    //        _moveCommand->setAfterPos(nodes);
    //        SwwKnob::CommandStack::pushCommand(_moveCommand);
    //        _moveCommand = 0;
    //        //*********************************************************
    //        _positionChanged = false;
    //    }

    //    _clicked = false;

    QGraphicsItem::mouseReleaseEvent(event);
}

void NodeItem::createKnobGroup()
{
    if(_knobGroup) return;

    _knobGroup = new KnobGroupWidget(0);
    foreach(PlugItem* p,_plugItemList)
    {
        createKnob(p->plug());
    }//foreach

}

void NodeItem::updateKnobs()
{
    if(_knobGroup && !_knobGroup->isHidden()){
        Q_FOREACH(KnobBaseWidget* knobBase,_visibleKnobs){
            if(!knobBase->isHidden()){
                knobBase->knob()->updateKnob();
            }
        }
    }
}

void NodeItem::createKnob(Sww::Core::PlugBase* plug)
{
    Q_ASSERT_X(_knobGroup,_node->getName().c_str(),"empty knob group.");

    _knobGroup->setNodeName(_node->getName().c_str());

    if(!plug->isHidden())
    {
        //Knobの要素を生成
        KnobObject* knobObject;
        bool valid =  KnobMaker::create(plug,knobObject,0);

        KnobBaseWidget* knobBase = new KnobBaseWidget();
        knobBase->setKnob(knobObject);

        _knobGroup->addKnobBase(knobBase);

        QObject::connect(plug,SIGNAL(plugStateChanged()),knobObject,SLOT(updateKnob()));
        _visibleKnobs << knobBase;
    }
}

void NodeItem::removeKnob(const Sww::Core::PlugMetaPtr &plugMeta)
{
    Q_ASSERT(_knobGroup);
    if(!plugMeta->isHidden)
    {
        KnobBaseWidget* knobBase = 0;
        Q_FOREACH(KnobBaseWidget* knob,_visibleKnobs){
            if(knob->hasKnob(plugMeta->name))knobBase = knob;
        }

        Q_ASSERT_X(knobBase,plugMeta->name.c_str(),"unexpected."); //登録されていないvisible knobを消そうとしている場合

        _knobGroup->removeWidget(knobBase);
        _visibleKnobs.removeOne(knobBase);
        delete knobBase;
    }
}

void NodeItem::createContextMenu()
{
    Q_ASSERT_X(!nodeGraphScene()->views().isEmpty(),
               _node->getName().c_str(),
               "create context menu failed.");

    QWidget* parent = dynamic_cast<QWidget*>(nodeGraphScene()->views()[0]);//ひとまずnodeGraphWidgetを親とする
    Q_ASSERT(parent);
    _contextMenu = new QMenu(parent);

    QAction *addDynamicPlugAction = new QAction("add dynamic plug",_contextMenu);
    _contextMenu->addAction(addDynamicPlugAction);

    QAction *deleteNodeAction = new QAction("delete",_contextMenu);
    _contextMenu->addAction(deleteNodeAction);

    QAction *showInfoNodeAction = new QAction("show info",_contextMenu);
    _contextMenu->addAction(showInfoNodeAction);

    connect(showInfoNodeAction,SIGNAL(triggered()),this,SLOT(showInfo()));
    connect(deleteNodeAction,SIGNAL(triggered()),this,SLOT(removeMyself()));
    connect(addDynamicPlugAction,SIGNAL(triggered()),this,SLOT(addUserDefinedDynamicPlug()));

}

void NodeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if(!_contextMenu)createContextMenu();
    _contextMenu->exec(event->screenPos());
}

void NodeItem::addDynamicPlugItem(Sww::Core::PlugBase *plugPtr)
{
    int current =
            _node->inputConnectablePlugCount() +
            _node->outputConnectablePlugCount() +
            _node->inputConnectablePlugCountDynamic() +
            _node->outputConnectablePlugCountDynamic() - 1; //カウントアップした後なのでマイナス1

    addDynamicPlugItem(plugPtr,current);

    prepareGeometryChange();
    update();

}

void NodeItem::addDynamicPlugItem(Sww::Core::PlugBase *plugPtr ,int current)
{
    PlugItem* item = setPlugItem(plugPtr,current);
    if(plugPtr->isConnectable())_connectablePlugItemListDynamic << item;

    int totalConnectablePlugCount = current + 1;
    _nodeHeight = _plugNameOrgY + _plugNameHeight *  totalConnectablePlugCount;

    createKnob(plugPtr); //knob更新

}

void NodeItem::deleteDynamicPlugItem(const Sww::Core::PlugMetaPtr &plugMeta)
{
    if((!plugMeta->isConnectable) && plugMeta->isHidden) {
        //do nothing.
    }else{
        PlugItem* plugItem = _plugNamesContainer.take(plugMeta->name.c_str());
        _plugItemList.removeOne(plugItem);
        if(plugMeta->isConnectable)_connectablePlugItemListDynamic.removeOne(plugItem);

        int totalConnectablePlugCount =
                _node->inputConnectablePlugCount() +
                _node->outputConnectablePlugCount() +
                _node->inputConnectablePlugCountDynamic() +
                _node->outputConnectablePlugCountDynamic();

        //dynamicPlugItem位置を補正******
        int step = 0;
        foreach(PlugItem* item,_connectablePlugItemListDynamic)
        {
            int current = _staticConnectablePlugItemCount + step;
            float y = (current * _plugNameHeight)  + _plugNameOrgY;
            item->setY(y);
            step++;
        }
        //*****************************

        //connector削除　直接nodeGraphScene()->disconnectGui(cnt)をたたくと、削除しながらイテレートしてしまうので、いったんリストを移す******
        QList<Connector*> deleteConnectorList;
        foreach(Connector* cnt,_connectorList)
        {
            if(cnt->hasPlugItem(plugItem))deleteConnectorList << cnt;
        }
        foreach(Connector* cnt,deleteConnectorList)
        {
            nodeGraphScene()->disconnectGui(cnt);
        }
        //******************************************************************************************************

        _nodeHeight = _plugNameOrgY + _plugNameHeight *  totalConnectablePlugCount; //高さ再計算

        removeKnob(plugMeta); //knob更新

        delete plugItem;
        prepareGeometryChange();
        update();
    }
}

void NodeItem::addUserDefinedDynamicPlug(){
    QWidget* parent = dynamic_cast<QWidget*>(nodeGraphScene()->views()[0]);//ひとまずnodeGraphWidgetを親とする
    Q_ASSERT(parent);

    AddDynamicPlugWidget* w = new AddDynamicPlugWidget(node(),parent);
    Qt::WindowFlags flags = w->windowFlags();
    w->setWindowFlags(flags|Qt::Window);
    w->show();
}

void NodeItem::updateConnectors()
{
    foreach(Connector *connector, _connectorList)
    {
        connector->updateLine();
    }
}

void NodeItem::connectPlugItem(const Sww::Core::ConnectState *con){
    Q_ASSERT(con);
    Q_ASSERT(_scene);

    if(!_scene->connect(con->outNodeName.c_str(),
                        con->outPlugName.c_str(),
                        con->inNodeName.c_str(),
                        con->inPlugName.c_str())){
        SWW_ERR("connect failed. " << con->outNodeName << "."
               << con->outPlugName << "->"
               << con->inNodeName << "."
               << con->inPlugName);
    }

    //SWW_DEBUG("connect gui");
}

void NodeItem::recieveErrorOccurred(bool occurred){
    update();
}

QVariant NodeItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene()) {
        QPointF pos = this->pos();
        _node->posInGraph(Imath::V2f(pos.x(),pos.y()));
        _scene->nodeItemIsMoved(true);
    }
    return QGraphicsItem::itemChange(change, value);
}

void NodeItem::updateNodeItem(){
//    Imath::V2f& pos = _node->posInGraph();
    Imath::V2f pos = _node->posInGraph();
    setPos(pos.x,pos.y);
    updateConnectors();
    //if(_knobGroup && !_knobGroup->isHidden())_knobGroup->updateChildren();
    updateKnobs();
}

QRectF NodeItem::sceneBoundingRectWithTitleItem()const{
    QRectF result = sceneBoundingRect().united(_titleItem->sceneBoundingRect());
    return result;
}

//void NodeItem::initFont(){
//    QGraphicsTextItem* textItem = new QGraphicsTextItem();
//    QFont font;
//    font.setFamily(fontFamilyNodeHeader);
//    textItem->setFont(font);
//}
