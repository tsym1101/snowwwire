#include "Core/PlugBase.h"
#include "SwwMacro.h"
#include "VectorHelper.h"

#include <iostream>
#include "NodeBase.h"
#include "SwwAssert.h"

#include "Node.h"
#include "Core/RootNode.h"
#include "Core/PlugRegister.h"
#include "Core/ConnectHandler.h"

#include "Core/Node.h"

SWW_CORE_NAMESPACE_BEGIN

//class Node;

PlugMeta::PlugMeta(PlugBase* p):name(p->getName()),
    isConnectable(p->isConnectable()),
    isHidden(p->isHidden())
{
}

PlugBase::PlugBase(const std::string& name):
    _isClean(false),
    _isOutput(false),
    _isMultiConnect(false),
    _isAutoUpdate(false),
    _isStorable(false),
    _isDynamic(false),
    _isUserDefined(false),
    _isHidden(false),
    _isConnectable(false),
    _isPassthrough(false),
    _isInheritedAndHidden(false),
    _index(-1),
    /*_name(name),*/_node(0),_knobTypeDesc(0),
    _firstPushDirtyCalled(false),
    _isExternalConnection(false)
{
    setName(name);
}

PlugBase::~PlugBase(){
}

void PlugBase::setOutput(bool b){
    _isOutput = b;
}

bool PlugBase::connectOk(PlugBase *plug)
{
    if(isFull()){
        SWW_WARN(_node->getName().c_str() << "->" <<  getName().c_str() << "plug is full.") ;
        return false;
    }

    return true;
}

bool PlugBase::match(PlugBase *plug){
    if(getSwwTypeDesc() != plug->getSwwTypeDesc())
    {
        SWW_ERR("type desc mismatch." << getDebugName().c_str() << "&" << getDebugName().c_str() );
        return false;
    }
    return true;
}

void PlugBase::setClean(){
    _isClean = true;
}

void PlugBase::setDirty(){
    _isClean = false;
}

void PlugBase::pushDirty(bool lock)
{
    if(!isClean() && _firstPushDirtyCalled){
        if(isOutput() && isAutoUpdate() && !_next.size())
        {
            bool result = pull();
            if(!result){
                SWW_ERR("pull failed. : "  << getDebugName());
            }
        }
        return;
    }

    if(lock){//for mutex locker.
        bool ok = _node->mutex()->tryLock();
        Q_ASSERT_X(ok,getDebugName().c_str(),"unexpected mutex lock in pushDirty");
//        QMutexLocker locker(_node->mutex());
        setDirty();
        _node->mutex()->unlock();
    }else{
//        Q_ASSERT_X(0,getDebugName().c_str(),"push dirty without lock is unexpected");
        setDirty();
    }

    for(auto c:_next){
        PlugBase* connectedPlug = static_cast<PlugBase*>(c);
        connectedPlug->pushDirty(lock);
    }

    if(isOutput() && isAutoUpdate() && !_next.size())
    {
        bool result = pull();
        if(!result){
            SWW_ERR("pull failed. : "  << getDebugName());
            //setClean(); //失敗しても、再びpushDirtyするため
        }
    }

    _firstPushDirtyCalled = true;
}

bool PlugBase::disconnect(PlugBase *plug , bool withCleanUp){

    if(_node == plug->_node){
        unaffects(plug);
        return true;
    }

    {//mutex
        QMutexLocker locker(plug->getNode()->mutex());
        removeNext(plug);
        if(plug->cloneWhenDisconnect() ){
            cloneDataTo(plug);
        }
        else {
            plug->resetData();
        }
    }

    if(withCleanUp){
        bool stat = plug->postSetRoutine();
        if(!stat){
            SWW_WARN("postSetRoutine failed after disconnect." << plug->getDebugName());
        }

        stat = plug->postConnectionChanged(false);
        if(!stat){
            SWW_WARN("postConnectionChanged failed after disconnect." << plug->getDebugName());
        }

        plug->setExternalConnection(false);
    }

    return  true;
}

void PlugBase::prepareDestroy()
{
    //copy vector.
    std::vector<Chainable*> prev = _prev;
    std::vector<Chainable*> next = _next;

    //int size = next.size();

    for(auto c : next){
        //PlugBase* p = static_cast<PlugBase*>(c);
        PlugBase* p = dynamic_cast<PlugBase*>(c);
        disconnect(p);
    }

    for(auto c : prev){
        //PlugBase* p = static_cast<PlugBase*>(c);
        PlugBase* p = dynamic_cast<PlugBase*>(c);
        p->disconnect(this,false);
    }
}

bool PlugBase::pull(bool locked)
{
    Q_ASSERT_X(isOutput(),getDebugName().c_str(),"pull in input plug.");

    if(_prev.size() == 0){

        if(locked){
            QMutexLocker locker(_node->mutex());

            if(!getNode()->execute(this)){
                getNode()->setErrorOccurred(true);
                return false;
            }
            getNode()->setErrorOccurred(false);
            setClean();
        }
        else{
            if(!getNode()->execute(this)){
                getNode()->setErrorOccurred(true);
                return false;
            }
            getNode()->setErrorOccurred(false);
            setClean();
        }
        return true;
    }else{

        for(auto c : _prev){
            PlugBase* p0 =  static_cast<PlugBase*>(c);
            if(sameParent(p0))continue;
            if(!p0->pull()){
                SWW_ERR("pull in connectedPlugsNegative failed. " << p0->getDebugName());
                getNode()->setErrorOccurred(true);
                return false;
            }
        }
        getNode()->setErrorOccurred(false);
        setClean();
        return true;
    }
}

bool PlugBase::isFull()const{
    //return !isOutput() && !isMultiConnect() && !_connectedPlugs.empty();
    return !isOutput() && !isMultiConnect() && !_prev.empty();

}

bool PlugBase::postSetRoutine(bool lock)
{
    //    lock = false;
    //Q_ASSERT(!isOutput());
    //    if(_node->getName() == "GLWriteBuffer_1"){
    //        SWW_DEBUG("hoge");
    //    }

    if(!isOutput()){
        bool stat = true;
        stat = _node->postSetProcess(this);
        if(!stat) return false;
        for(auto c : _next){
            PlugBase* p =  static_cast<PlugBase*>(c);
            //            if(p->_node->getName() == "GLWriteBuffer_1" && p->getName() == "frame"){
            //                SWW_DEBUG("hoge");
            //            }
            SWW_CHECK_BOOL(p->postSetRoutine(lock));
        }
        pushDirty(lock);
    }
    return true;
}

bool PlugBase::postConnectionChanged(bool connected)
{
    return _node->postConnectionChanged(this,connected);
}

std::string PlugBase::getDebugName() const
{
    Q_ASSERT_X(_node,getName().c_str(),"parent node is invalid.");
    return _node->getName() + "." + getName();
}

void PlugBase::printConnections()const
{
    SWW_DEBUG(getName() << " " << " connections **************************************");

    for(auto c : _prev){
        PlugBase* p =  static_cast<PlugBase*>(c);
        SWW_DEBUG("prev   " << p->getPath());
    }

    for(auto c : _next){
        PlugBase* p =  static_cast<PlugBase*>(c);
        SWW_DEBUG("next " << p->getPath());
    }


}

void PlugBase::getConnPlugNames(const std::vector<Chainable*>& target,
                                std::vector<std::string>& names,
                                bool getInternal) const{
    for(auto c : target){
        PlugBase* p =  static_cast<PlugBase*>(c);

        if(p->getNode() == getNode())continue; //affectsしてるだけの場合はスキップ

        if(!getInternal){
            if(p->_node->isInternal()) continue;//接続先がinternalならスキップ
        }
        if(dynamic_cast<RootNode*>(p->_node)){//接続先がRootNodeならスキップ
            continue;
        }

        std::string str;

        if(!inSameGraph(p)){
            Q_ASSERT(dynamic_cast<Node*>(getNode()));
            str = dynamic_cast<Node*>(getNode())->getRelativePath(p->getNode()->getPath());

            //保有する親ノード起点の相対パスになるのでノード名が先頭に必要
            str =  getNode()->getName() + "/" +  str + "." + p->getName();
            //str =  std::string("%this") + "/" +  str + "." + p->getName();

        }else{
            //保有する親ノード起点の相対パスになるのでノード名が先頭に必要
            str = p->getDebugName();
        }

        names.push_back(str);
    }
}

void PlugBase::getConnPlugNames(std::vector<std::string>& prevNames,
                                std::vector<std::string>& nextNames,
                                bool getInternal) const
{
    getConnPlugNames(_prev,prevNames,getInternal);
    getConnPlugNames(_next,nextNames,getInternal);


}

void PlugBase::getConnPlugNames(std::vector<std::string>& names,
                                bool getPrev,
                                bool getInternal) const
{
    if(getPrev)getConnPlugNames(_prev,names,getInternal);
    else getConnPlugNames(_next,names,getInternal);
}

std::string PlugBase::getPath()const{
    Q_ASSERT_X(_node,getName().c_str(),"null node.");
    std::string result = _node->getPath();
    return result + "." + getName();
}

bool PlugBase::inSameGraph(const PlugBase *p) const{
    //    Q_ASSERT_X(p,"inSameGraph",getDebugName().c_str());
    //    Q_ASSERT_X(dynamic_cast<Node*>(_node),"inSameGraph",getDebugName().c_str());
    //    Q_ASSERT_X(dynamic_cast<Node*>(p->_node),"inSameGraph",p->getDebugName().c_str());
    bool result = static_cast<Node*>(_node)->inSameGraph(static_cast<Node*>(p->_node));

    return result;
}

bool PlugBase::isExternalConnection()const{
    return _isExternalConnection;
}

void PlugBase::setExternalConnection(bool b){
    _isExternalConnection = b;
}

void PlugBase::emitPlugStateChanged(){
    Q_EMIT plugStateChanged();
}

bool PlugBase::raisePlug(){
    Node* thisNode = dynamic_cast<Node*>(_node);
    if(!thisNode)return false;

    Node* node = thisNode->parentNode();

    if(dynamic_cast<RootNode*>(node)){
        SWW_ERR("in root. do nothing.");
        return false;
    }

    std::string newPlugName;
    if(isOutput()){newPlugName = "__" + getName();}
    else{newPlugName =   getName() + "__";}

    PlugBase* p = PlugRegister::create(getSwwTypeDescString(),
                                                       newPlugName);

    if(!p){
        SWW_ERR("raise failed. " << getDebugName());
        return false;
    }

    p->setOutput(isOutput());
    p->setStorable(isStorable());
    p->setConnectable(isConnectable());
    p->setHidden(isHidden());
    p->setMultiConnect(isMultiConnect());
    p->setUserDefined(true);

    if(!node->addDynamicPlug(p)){
        SWW_ERR("raise failed. " << getDebugName());
        delete p;
        return false;
    }

    if(!isOutput()){
        ConnectHandler hnd(p,this);
        if(!hnd.exec()){
            SWW_ERR("raise failed. " << getDebugName());
            return false;
        }
    }else
    {
        //output同士は接続を反転
        Q_ASSERT_X(p->isOutput(),this->getName().c_str(),"invalid connection.");
        ConnectHandler hnd(this,p);
        if(!hnd.exec()){
            SWW_ERR("raise failed. " << getDebugName());
            return false;
        }
    }
}

QString PlugBase::getValueStr(){
    QString valueStr = getName().c_str() + QString(" ") + valueToString();
    return valueStr;
}

SWW_CORE_NAMESPACE_END
