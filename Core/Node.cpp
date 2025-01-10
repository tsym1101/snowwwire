#include "Core/Node.h"
#include <QDebug>
#include "Core/SwwMacro.h"
#include "Core/NodeGraph.h"
#include "Core/ConnectHandler.h"
#include "Core/PlugBase.h"
#include "Core/VectorHelper.h"
#include "Core/NodeRegister.h"
#include "PlugBase.h"
#include "Core/RootNode.h"
#include "Core/PlugType.h"
#include <QDir>
#include <QTextCodec>

SWW_CORE_NAMESPACE_BEGIN

bool /*NodeGraph*/ Node::createNode(const std::string &nodeType)
{
    Node* node;
    return createNode(nodeType,node);
}

bool /*NodeGraph*/ Node::createNode(const std::string &nodeType, Node *&nodePtr,const std::string &name)
{
    //ノード命名**********************************************************
    std::string myName;
    if(name.empty()){//ノード名を指定せずに生成した時
        myName = genUniqueNodeName(nodeType,false);
    }else{//ノード名を指定して生成した時
        myName = genUniqueNodeName(name);
    }
    //**********************************************************

    bool valid;
    valid = NodeRegister::create(myName,nodeType,nodePtr,this);

    if(!valid)
    {
        SWW_ERR(nodeType << ": get creatorFunction failed.");
        nodePtr = 0;    //失敗したらポインタにnullを代入
        return valid;
    }

    Node::Ptr node(nodePtr);
    _children[myName] = node;

    return true;
}

std::string /*NodeGraph*/ Node::genUniqueNodeName(const std::string &baseName,bool useBaseNameIfUnique){
    if(useBaseNameIfUnique){
        if(!_children.contains(baseName)){
            return baseName;
        }
    }

    int i = 1;
    while(1)    //名前がダブらずに追加できるまで
    {
        std::string myName = baseName + std::string("_") + QString::number(i).toStdString();
        if(!_children.contains(myName)){
            return myName;
        }
        i++;
    }
}


bool /*NodeGraph*/ Node::removeNode(std::string nodeName)
{
    Node::Ptr node = _children.value(nodeName);

    if(!node)
    {
        SWW_ERR(nodeName << " doesn't exist.");
        return false;
    }

    _children.remove(nodeName);

    return true;
}

bool /*NodeGraph*/ Node::connect(PlugBase* p1,PlugBase* p2)//p1 = output, p2 = input
{
    if(!p1->addNext(p2)){
        SWW_ERR("connect failed. " << p1->getDebugName() << "->" << p2->getDebugName());
        return false;
    }

    if(!p2->isOutput()){
        if(!p2->postSetRoutine()){
            SWW_ERR("postSetRoutine failed." << p2->getDebugName());
            disconnect(p1,p2);
            return false;
        }
        if(!p2->postConnectionChanged(true)){
            SWW_ERR("postConnectionChanged failed." << p2->getDebugName());
            disconnect(p1,p2);
            return false;
        }
    }
    else{
        p2->pushDirty();
    }

    if(!p1->inSameGraph(p2)){
        p2->setExternalConnection(true);
    }

    return true;
}

bool /*NodeGraph*/ Node::connect(std::string outputNodeName, const std::string &outputPlugName,
                                 std::string inputNodeName, const std::string &inputPlugName)
{

    Node* outputNodePtr = /*_parentNode->*/getNodeFromPath(outputNodeName);
    if(!outputNodePtr) return false;
    Node* inputNodePtr = /*_parentNode->*/getNodeFromPath(inputNodeName);
    if(!inputNodePtr) return false;

    PlugBase::Ptr outputPlug;
    PlugBase::Ptr inputPlug;

    //接続ノードを名前で取得
    //outputPlug = outputNodePtr->_plugMap.value(outputPlugName);
    outputPlug = outputNodePtr->_plugArray.value(outputPlugName);
    if(!outputPlug){
        SWW_ERR(" has no plug. " << outputNodeName << ":" << outputPlugName);
        return false;
    }

    //inputPlug = inputNodePtr->_plugMap.value(inputPlugName);
    inputPlug = inputNodePtr->_plugArray.value(inputPlugName);
    if(!inputPlug){
        SWW_ERR(" has no plug. " << inputNodeName << ":" << inputPlugName);
        return false;
    }

    return connect(outputPlug.get(),inputPlug.get());
}

bool /*NodeGraph*/ Node::disconnect(PlugBase *p1,PlugBase* p2) //p1 = output, p2 = input
{
    {//mutex
        QMutexLocker locker(p2->getNode()->mutex());
        //p1->disconnect(p2);
        p1->removeNext(p2);
        if(p2->cloneWhenDisconnect() ){
            p1->cloneDataTo(p2);
        }
        else {
            p2->resetData();
        }
    }

    bool stat = p2->postSetRoutine();
    if(!stat){
        SWW_WARN("postSetRoutine failed after disconnect." << p2->getDebugName());
    }

    stat = p2->postConnectionChanged(false);
    if(!stat){
        SWW_WARN("postConnectionChanged failed after disconnect." << p2->getDebugName());
    }

    p2->setExternalConnection(false);

    return true;

    //return  p1->disconnect(p2);
}

void /*NodeGraph*/ Node::printAllNodes()
{
    std::string str("current nodes : ");

    //std::map<std::string, Node::Ptr>::iterator it = _children.begin();
    QMap<std::string, Node::Ptr>::iterator it = _children.begin();
    while( it != _children.end() )
    {
        Node::Ptr& node = it.value();
        str = str + node->getName() + ",";
        ++it;
    }

    std::cout << str << std::endl;
}

void /*NodeGraph*/ Node::printClean()
{
    std::cout << "clean state -----------------------------------" << std::endl;
    //std::map<std::string, Node::Ptr>::iterator it = _children.begin();
    QMap<std::string, Node::Ptr>::iterator it = _children.begin();
    while( it != _children.end() )
    {
        Node::Ptr& node = it.value();
        node->printCleanState();

        ++it;
    }
}

void /*NodeGraph*/ Node::printAllInternalConnection()
{
    std::cout  << "All internal connections ..." << std::endl;
    //std::map<std::string, Node::Ptr>::iterator it = _children.begin();
    QMap<std::string, Node::Ptr>::iterator it = _children.begin();
    while( it != _children.end() )
    {
        Node::Ptr& node = it.value();
        node->printIntenalConnection();
        ++it;
    }
}

void /*NodeGraph*/ Node::add2ConnectHandler(ConnectHandler *c,int index)
{
    _indexedConnectHndList[index].push_back(c);
}

bool /*NodeGraph*/ Node::connectByHandler()
{
    int tmp = 0;
    std::map<int,std::vector<ConnectHandler*> >::iterator it = _indexedConnectHndList.begin();
    while( it != _indexedConnectHndList.end())
    {
        std::vector<ConnectHandler*> connectHndList = it->second;
        for(std::vector<ConnectHandler*>::iterator i = connectHndList.begin(); i != connectHndList.end() ; ++i){
            ConnectHandler* c = *i;
            bool valid = c->exec();
            if(!valid)SWW_ERR("connect failed.");
            delete c;
        }
        connectHndList.clear();
        ++it;
        tmp++;
    }
    _indexedConnectHndList.clear();

    //SWW_DEBUG("connectByHandler in " <<  _parentNode->getName());

    return true;
}

Node::Node():_inputConnectablePlugCount(0),_outputConnectablePlugCount(0)/*,_isRunner(false)*/,
    _inputConnectablePlugCountDynamic(0),_outputConnectablePlugCountDynamic(0),_posInGraph(0,0),_rootNode(0),_parentNode(0),
    _inSetValuePhase(false),_inPreInitialConnectPhase(false),_inConnectPhase(false),_inSetDynamicValuePhase(false)
  //,_inputPlugCount(0),_outputPlugCount(0),_inputPlugCountDynamic(0),_outputPlugCountDynamic(0)
{
    //_nodeGraph.reset(new NodeGraph(this));
}

Node::~Node()
{
}

//初期化処理 ノードのひな形を作成する
void Node::addPlug(PlugBase *plug)
{    


    Q_ASSERT_X(!_plugArray.contains(plug->getName()),
               (getName() + std::string(":") + plug->getName()).c_str() ,"already exist.");


    plug->index(0); //dummy
    PlugBase::Ptr ptr(plug);
    _plugArray.push_back(ptr);
    if(plug->isConnectable()){
        if(!plug->isOutput())_inputConnectablePlugCount++;
        else _outputConnectablePlugCount++;
    }

    plug->setNode(this);
}

bool Node::deleteDynamicPlugItem(const std::string& plugName){

    PlugBase::Ptr pDelete = _plugArray.value(plugName);

    if(!pDelete || !pDelete->isDynamic()){
        SWW_ERR("nothing to delete. " << plugName << " in " << getName());
        return false;
    }

    vectorErase(_runnerPlugContainer,pDelete.get());
    //if(_runnerPlugContainer.empty())_isRunner = false;

    if(!pDelete->isOutput())_inputConnectablePlugCountDynamic--;
    else _outputConnectablePlugCountDynamic--;

    PlugMetaPtr meta(new PlugMeta(pDelete.get()));
    emit emitDeleteDynamicPlug(meta);

    _plugArray.erase(plugName);


    return true;
}

void Node::destroyDynamicPlugItemArray()
{

    std::vector<std::string> deleteList;
    for(auto ptr : _plugArray){
        if(ptr->isDynamic()){
            deleteList.push_back(ptr->getName());
        }
    }

    for(auto str : deleteList){
        deleteDynamicPlugItem(str);
    }

    Q_ASSERT(!_inputConnectablePlugCountDynamic);
    Q_ASSERT(!_outputConnectablePlugCountDynamic);


}

void Node::printIntenalConnection()
{
    //    SWW_DEBUG(getName()  << "*******************************************" );

    //    SWW_DEBUG("INPUT");
    //    PlugBasePtrVec::const_iterator it = _plugArrayInput.constBegin();
    //    while(it != _plugArrayInput.end())
    //    {
    //        const PlugBase::Ptr& p = *it;
    //        p->printConnections();
    //        ++it;
    //    }

    //    SWW_DEBUG("OUTPUT");
    //    it = _plugArrayOutput.begin();
    //    while(it != _plugArrayOutput.end())
    //    {
    //        const PlugBase::Ptr& p = *it;
    //        p->printConnections();
    //        ++it;
    //    }

    Q_ASSERT(0); //not implemented.
}

void Node::printCleanState()
{
    //    std::cout << getName()  << "*******************************************" << std::endl;


    //    std::cout << "INPUT" << std::endl;
    //    PlugBasePtrVec::const_iterator it = _plugArrayInput.constBegin();
    //    while(it != _plugArrayInput.constEnd())
    //    {
    //        const PlugBase::Ptr& p = *it;
    //        std::string clean;
    //        if(p->isClean())clean = " clean";
    //        else clean = " dirty";
    //        std::cout << p->getName() << clean << std::endl;
    //        ++it;
    //    }

    //    std::cout << "OUTPUT" << std::endl;
    //    it = _plugArrayOutput.constBegin();
    //    while(it != _plugArrayOutput.constEnd())
    //    {
    //        const PlugBase::Ptr& p = *it;
    //        std::string clean;
    //        if(p->isClean())clean = " clean";
    //        else clean = " dirty";
    //        std::cout << p->getName() << clean << std::endl;
    //        ++it;
    //    }
    Q_ASSERT(0); //not implemented.
}

void Node::getConnPlugList(std::vector<std::string> &prevList,
                           std::vector<std::string> &nextList,
                           bool getInternal) const{

    for (PlugBasePtrVec::const_iterator i = _plugArray.constBegin();i != _plugArray.constEnd();++i)
    {
        const PlugBase::Ptr& p = *i;

        std::vector<std::string> prevNames;
        std::vector<std::string> nextNames;
        p->getConnPlugNames(prevNames,nextNames,getInternal);

        std::string connector;
        for(std::vector<std::string>::iterator i = prevNames.begin(); i != prevNames.end() ; ++i){
            const std::string& str = *i;
            connector = connector + std::string(" ") + str;
        }
        if(!connector.empty()){
            connector = p->getName() + connector;
            prevList.push_back(connector);
        }
        connector = std::string();
        for(std::vector<std::string>::iterator i = nextNames.begin(); i != nextNames.end() ; ++i){
            const std::string& str = *i;
            connector = connector + std::string(" ") + str;
        }
        if(!connector.empty()){
            connector = p->getName() + connector;
            nextList.push_back(connector);
        }
    }
}

void Node::getValueList(QStringList &list , bool getDynamic)
{
    for(PlugBase::Ptr& p :_plugArray){
        if(!p->isOutput()  && getDynamic == p->isDynamic()){

            if(!p->isStorable()) continue;
            if(p->prevCount()) continue;

            QString value = p->valueToString();
            if(!value.isNull())
            {
                QString result = p->getValueStr();
                list << result;
            }
        }
    }
}

bool Node::setValueString(const QString &plugName, const QString &value)
{
    if(!_plugArray.contains(plugName.toStdString())){
        SWW_ERR(getName() << " " << plugName.toStdString() << " not exist.");
        return false;
    }

    PlugBase::Ptr p = _plugArray.value(plugName.toStdString());
    //if(!getInputPlug(plugName.toStdString(),p))return false;
    return p->setValueString(value);
}

void Node::getUserDefinedDynamicPlugs(std::vector<PlugBase *> &plugs){
    for (PlugBasePtrVec::iterator i = _plugArray.begin();i != _plugArray.end();++i)
    {
        PlugBase::Ptr& p = *i;
        if(p->isDynamic() && p->isUserDefined()){
            plugs.push_back(p.get());
        }
    }

}

void Node::setRunner(PlugBase *p)
{
    //_isRunner = true;
    _runnerPlugContainer.push_back(p);
}


bool Node::addDynamicPlug(PlugBase* plug)
{
    if(_plugArray.contains(plug->getName()))
    {
        SWW_ERR("addDynamicPlug failed.  plug name already exists in plug map." << getName() << ":" << plug->getName());
        return false;
    }

    if(!plug->isOutput()){
        // plug->index(_plugArrayInputDynamic.size()); //インデックスを追加する 追加前なので-1はいらない
        PlugBase::Ptr ptr(plug);
        _plugArray.push_back(ptr);
        if(plug->isConnectable())
        {
            _inputConnectablePlugCountDynamic++;
        }
    }else{
        // plug->index(_plugArrayOutputDynamic.size()); //インデックスを追加する 追加前なので-1はいらない
        PlugBase::Ptr ptr(plug);
        _plugArray.push_back(ptr);
        if(plug->isConnectable())
        {
            _outputConnectablePlugCountDynamic++;
        }
    }

    //_plugMap[plug->getName()] = plug;

    plug->setNode(this);
    plug->setDynamic(true);

    emit emitAddDynamicPlug(plug);

    return true;
}

bool Node::connectToGlobalTime(PlugBase* plug)
{
    /*NodeGraph*/ Node::connect(_rootNode->_framePlug,plug);
    return true;
}

void Node::hideInheritedPlug(PlugBase* p ,bool notStoreble)
{
    Q_ASSERT(!p->isInheritedAndHidden());

    if(!p->isOutput())
    {
        //Q_ASSERT(_plugArrayInput.contains(p->getName()));
        if(p->isConnectable())_inputConnectablePlugCount--;
    }else
    {
        //Q_ASSERT(_plugArrayOutput.contains(p->getName()));
        if(p->isConnectable())_outputConnectablePlugCount--;
    }

    p->setHidden(true);
    p->setConnectable(false);
    if(notStoreble)p->setStorable(false);

    p->setInheritedAndHidden(true);
}

void Node::showInheritedPlug(PlugBase* p , bool storeble)
{
    Q_ASSERT(p->isInheritedAndHidden());

    if(!p->isOutput())
    {
        //Q_ASSERT(_plugArrayInput.contains(p->getName()));
        _inputConnectablePlugCount++;
    }else
    {
        //Q_ASSERT(_plugArrayOutput.contains(p->getName()));
        _outputConnectablePlugCount++;
    }

    p->setHidden(false);
    p->setConnectable(true);
    if(storeble)p->setStorable(true);

    p->setInheritedAndHidden(false);
}

bool Node::hasPlug(const std::string &plugName) const{
    return _plugArray.contains(plugName);
}

void Node::storeDynamicPlugValueState(){

    _dynamicPlugValueState.clear();
    for (PlugBasePtrVec::iterator i = _plugArray.begin();i != _plugArray.end();++i)
    {
        PlugBase::Ptr& p = *i;
        if(!p->prevCount() && !p->isOutput() && p->isDynamic()){
            const std::string& nodeName = p->getName();
            std::string valueStr = p->valueToString().toStdString();

            if(!valueStr.empty())_dynamicPlugValueState[nodeName] = valueStr;
        }
    }
}

void Node::restoreDynamicPlugValueState(){

    for (std::map<std::string,std::string>::iterator i = _dynamicPlugValueState.begin(); i != _dynamicPlugValueState.end(); ++i)
    {
        const std::string& name =  i->first;
        const std::string& value =  i->second;
        setValueString(name.c_str(),value.c_str());
    }
}

void Node::storeDynamicPlugConnectState(){

    _dynamicPlugConnectState.clear();

    for (PlugBasePtrVec::iterator i = _plugArray.begin();i != _plugArray.end();++i)
    {
        PlugBase::Ptr& p = *i;

        if(!p->isDynamic()) continue;

        std::vector<std::string> prevNames,nextNames;
        p->getConnPlugNames(prevNames,nextNames,true);

        std::vector<std::string>& names = prevNames;
        if(p->isOutput())names = nextNames;

        for (std::vector<std::string>::iterator i = names.begin();i != names.end();++i)
        {
            std::string name = *i;

            QString nameTmp(name.c_str());
            QStringList split = nameTmp.split(".");
            QString nodeNameQt = split[0];
            QString plugNameQt = split[1];

            //マルチコネクトの場合、[インデックス]を除去
            QString pattern = "\\[\\d+\\]$"; //　\\d→数字 $→末尾
            QRegExp r(pattern);
            if(plugNameQt.contains(r)){
                plugNameQt = plugNameQt.remove(r);
            }

            ConnectState con;

            if(!p->isOutput()){
                con.outNodeName = nodeNameQt.toStdString();
                con.outPlugName = plugNameQt.toStdString();
                con.inNodeName = getName();
                con.inPlugName = p->getName();
            }
            else{
                con.inNodeName = nodeNameQt.toStdString();
                con.inPlugName = plugNameQt.toStdString();
                con.outNodeName = getName();
                con.outPlugName = p->getName();
            }

            _dynamicPlugConnectState.push_back(con);

        }
    }

}

void Node::restoreDynamicPlugConnectState(){
    for (std::vector<ConnectState>::iterator i = _dynamicPlugConnectState.begin();i != _dynamicPlugConnectState.end();++i)
    {
        ConnectState& con = *i;
        Q_EMIT emitConnectDynamicPlug(&con);
        //SWW_DEBUG("restore");
    }


}

void Node::setErrorOccurred(bool occurred){
    bool oldState = _errorOccurred;
    _errorOccurred = occurred;
    if(oldState != _errorOccurred)Q_EMIT emitErrorOccurred(occurred);
}

void Node::clearGraph(){
    _children.clear();
    Q_EMIT emitGraphCleared();
}

std::string Node::getPath()const{
    Node* parentNode = _parentNode;

    std::string result = getName();
    //SWW_DEBUG_VALUE(result);
    while(parentNode){
        const std::string& parentNodeName = parentNode->getName();
        result =  parentNodeName + "/" +result;
        parentNode = parentNode->parentNode();
    }

    //SWW_DEBUG_VALUE(result);

    QRegExp r("^\\$root/*");
    QString resultQt = result.c_str();
    resultQt.replace(r,"/");
    result = resultQt.toStdString();

    return result;
}

Node* Node::getNodeFromPath(const std::string& path)const
{
    Node* resNode = NULL;
    //QString tmpPath(path.c_str());

    QString  tmpPath = getAbsolutePath(path).c_str();

    //QRegExp regCheck("^/.+/$");// パスが"/"から始まり"/"で終わる
    QRegExp regCheck("^/");// パスが"/"から始まる
    if(!tmpPath.contains(regCheck)) {
        SWW_ERR("mismatch search path. " << path);
        SWW_ERR(tmpPath.toStdString());
        return NULL;
    }

    QRegExp regRoot("^/$");// パスが"/"の場合
    if (tmpPath.contains(regRoot)) {
        resNode = _rootNode;
    } else {
        QStringList pathChain = tmpPath.split("/");

        Q_ASSERT(!pathChain.isEmpty());

        pathChain.takeFirst();
        //pathChain.takeLast();

        Q_ASSERT_X(!pathChain.isEmpty(),path.c_str(),"empty.");

        //resNode = searchNode(list[list.count()-2].toStdString(), _rootNode->_children).get();
        resNode = _rootNode->searchNode(pathChain).get();
    }

    return resNode;
}

Node::Ptr Node::searchNode(QStringList &pathChain) const{
    QString nodeName = pathChain.takeFirst();
    Node::Ptr resNode = _children.value(nodeName.toStdString());
    if(!pathChain.empty()){
        if(resNode)resNode = resNode->searchNode(pathChain);
    }
    return resNode;
}

std::string Node::getRelativePath(const std::string &absPath) const{
    std::string thisPath = getPath();
    QDir tmp(thisPath.c_str());
    QString rPathQt = tmp.relativeFilePath(absPath.c_str());

    return rPathQt.toStdString();
}

std::string Node::getAbsolutePath(const std::string &relativepath) const{
    QRegExp regCheck("^/");// パスが"/"から始まる
    QString relativepathQt = relativepath.c_str();
    if(relativepathQt.contains(regCheck)) {
        return relativepath;
    }

    std::string thisPath = getPath();
    QRegExp regCheck0("^\.");// パスが"."から始まる

    QString absPathQt;

    if(relativepathQt.contains(regCheck0)){
        QRegExp regRemove("^\./");// パスが"."から始まる
        relativepathQt.remove(regRemove);
    }

    QString thisPathQt = thisPath.c_str();
    QRegExp regCheck1("/$");// パスが"/"で終わる
    if(thisPathQt.contains(regCheck1)){
        absPathQt = thisPathQt + relativepathQt;
    }else{
        absPathQt = thisPathQt + "/" + relativepathQt;
    }

    QString cleanPathQt = QDir::cleanPath(absPathQt);
    return cleanPathQt.toStdString();
}

bool Node::inSameGraph(Node *n)const{
    Q_ASSERT_X(n,"inSameGraph",getName().c_str());
    return _parentNode == n->_parentNode;
}

Imath::V2i Node::getGlobalFrameRange()const{
    return Imath::V2i(_rootNode->startFrame(),_rootNode->endFrame());
}

bool Node::hasChild(const std::string &nodeName) const{
    if(!_children.value(nodeName))return false;
    return true;
}

std::vector<PlugBase*> Node::getRunnerPlugs(){
    return _runnerPlugContainer;
}

void Node::emitNodeStateChanged(){
    Q_EMIT nodeStateChanged();
}

void Node::emitRemoveMyself(){
    Q_EMIT removeMyself();
}

void Node::emitSelectMyself(){
    Q_EMIT selectMyself();
}

void Node::emitDeselectChildren(){
    Q_EMIT deselectChildren();
}

QString Node::readTooltipFromResource(const QString& resourcePath) const{

    QFile file(resourcePath);

    if (!file.open(QIODevice::ReadOnly)){
        SWW_ERR("read " << resourcePath.toStdString() << " failed.");
        return QString();
    }

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("utf-8"));
    QString tooltipStr = in.readAll();

    return tooltipStr;
}

SWW_CORE_NAMESPACE_END
