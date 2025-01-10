#include "JsonSerializer.h"
#include "Core/Node.h"
#include "Core/RootNode.h".h"
#include "Core/SwwVersion.h"
#include "Core/PlugBase.h"

SWW_CORE_NAMESPACE_BEGIN

using namespace nlohmann;

JsonSerializer::JsonSerializer():_flags(None)
{
    //set default flags.
    _flags |= getChildren;
    _flags |= useType;
    _flags |= usePos;
    _flags |= useConn;
    _flags |= useValue;
    _flags |= useDValue;
    _flags |= useUdd;
}

SwwJson JsonSerializer::traverse(Node* node)const{

    SwwJson jResult;
    serializeProperty(node,jResult);

    if(_flags & getChildren){
        serializeChildren(node,jResult);
    }
    return jResult;
}

void JsonSerializer::serializeChildren(Node* node,SwwJson& j) const{

    QMap<std::string, Sww::Core::Node::Ptr>::iterator it = node->_children.begin();
    while( it != node->_children.end())
    {
        Sww::Core::Node::Ptr& nodePtr = it.value();
        if(nodePtr->isInternal()){
            ++it;
            continue;
        }
        SwwJson jChild = traverse(nodePtr.get());
        j[nodePtr->getName()] = jChild;
        ++it;
    }
}


std::string JsonSerializer::serializeRoot(Sww::Core::RootNode* rootNode)const{

    if(rootNode){
        SwwJson jProj;
        SwwJson j;

        j["version"] = Sww::Core::dsVersionStr();
        j["frameRange"] = {rootNode->startFrame(),rootNode->endFrame()};
        SwwJson jChildren;
        serializeChildren(rootNode,jChildren);
        j["$root"] = jChildren;

        jProj["ds_project"] = j;

        return jProj.dump(2);
    }else{
        SWW_ERR("root node is null.");
    }

    return std::string();
}

std::string JsonSerializer::serialize(const std::vector<Node*>& nodes)const{
    SwwJson j;
    for(std::vector<Node*>::const_iterator it = nodes.begin();it != nodes.end();++it){
        Node* node = *it;
        SwwJson jNodeGraph = traverse(node);
        j[node->getName()] = jNodeGraph;
    }

    return j.dump(2);
}

std::string JsonSerializer::serialize(PlugBase *plug){
    resetFlags();
    _flags |= asOverWriteMode;

    if(plug->isUserDefined()){
        _flags |= useUdd;
    }
    else if(plug->isDynamic()){
        _flags |= useDValue;
    }
    else{
        _flags |= useValue;
    }

    addToValueList(plug);
    std::vector<Node*> nodes;
    Node* nodeTmp = dynamic_cast<Node*>(plug->_node);
    Q_ASSERT(nodeTmp);
    nodes.push_back(nodeTmp);
    std::string result = serialize(nodes);
    _valueList.clear();

    return result;
}

void JsonSerializer::serializeProperty(Node* node, SwwJson& j)const{

    SwwJson jProperty;

    if(_flags & asOverWriteMode)jProperty["%ow"] = true;
    if(_flags & useType)jProperty["type"] = node->nodeTypeName();
    if(_flags & usePos)jProperty["pos"] = {node->posInGraph().x ,node->posInGraph().y};

    //接続情報記述 *****************************************************************
    if(_flags & useConn){
        std::vector<std::string> prevConn,nextConn;
        node->getConnPlugList(prevConn,nextConn,false);

        //接続先に自身が含まれる場合、予約語%thisに変換
        for(auto& conn:prevConn){
            QString connQt = conn.c_str();
            QString rep(QString(" ") + node->getName().c_str() + "/");
            connQt.replace(rep," %this/");
            conn = connQt.toStdString();
        }

        if(prevConn.size() != 0){
            jProperty["conn"] = prevConn;
        }
    }
    //****************************************************************************

    //value記述 *******************************************************************
    if(_flags & useValue){
        QStringList valueList;
        if(_valueList.empty()){
            node->getValueList(valueList,false);
        }else{
            valueList = _valueList;
        }
        if(!valueList.empty()){
            std::vector<std::string> valueVec;
            Q_FOREACH(QString str,valueList){
                valueVec.push_back(str.toStdString());
            }
            jProperty["value"] = valueVec;
        }
    }
    //****************************************************************************

    //dynamic value記述 ***********************************************************
    if(_flags & useDValue){
        QStringList dynamicValueList;
        if(_valueList.empty()){
            node->getValueList(dynamicValueList,true);
        }
        else{
            dynamicValueList = _valueList;
        }
        if(!dynamicValueList.empty()){
            std::vector<std::string> valueVec;
            Q_FOREACH(QString str,dynamicValueList){
                valueVec.push_back(str.toStdString());
            }
            jProperty["dValue"] = valueVec;
        }
    }

    if(_flags & useUdd){
        std::vector<Sww::Core::PlugBase*> uddPlugs;
        if(_valueList.empty()){
            node->getUserDefinedDynamicPlugs(uddPlugs);
        }
        else{
            Q_FOREACH(QString str,_valueList){
                PlugBase* p = node->_plugArray.value(str.toStdString()).get();
                if(p)uddPlugs.push_back(p);
            }
        }

        //user defined dynamic value(udd)**********************************************
        SwwJson jUddStatusRoot;
        for(std::vector<Sww::Core::PlugBase*>::iterator it = uddPlugs.begin();
            it != uddPlugs.end();++it){
            Sww::Core::PlugBase* p = (*it);
            SwwJson jUddStatus;
            jUddStatus["type"] = p->getSwwTypeDescString();
            jUddStatus["knobType"] = p->getKnobType();
            jUddStatus["output"] = p->isOutput();
            jUddStatus["connectable"] = p->isConnectable();
            jUddStatus["storable"] = p->isStorable();
            jUddStatus["hidden"] = p->isHidden();
            jUddStatus["multiConnect"] = p->isMultiConnect();
            jUddStatusRoot[p->getName()] = jUddStatus;
        }
        if(!jUddStatusRoot.is_null())jProperty["udd"] = jUddStatusRoot;
    }

    if(!jProperty.is_null())j["%pr"] = jProperty;
}

void JsonSerializer::setFlags(JsonSerializerFlags flag){
    _flags |= flag;
}

void JsonSerializer::resetFlags(){
    _flags = None;
}

void JsonSerializer::removeFlags(JsonSerializerFlags flag){
    _flags &= ~flag;//フラグ解除 ビット反転して論理積
}

void JsonSerializer::addToValueList(PlugBase *plug){
    QString valueStr =  plug->getName().c_str() + QString(" ") + plug->valueToString();
    _valueList.push_back(valueStr);
}

SwwJson JsonSerializer::makeCommand(const std::vector<std::string>& nodeNames, CommandType cType) {
    resetFlags();

    SwwJson result;
    SwwJson jCommand;

    switch (cType) {
    case kRemove:
    {
        jCommand["rm"] = nodeNames;
    }
        break;
    case kSelect:
    {
        jCommand["sel"] = nodeNames;
    }
        break;
    case kDeselect:
    {
        jCommand["desel"] = true;
    }
        break;
//    case kTransition:
//    {
//        std::string path = nodes[0]->getPath();
//        jCommand["tr"] = path;

//    }
//        break;
    default:
        break;
    }

    result["%cmd"] = jCommand;

    return result;
}

SWW_CORE_NAMESPACE_END
