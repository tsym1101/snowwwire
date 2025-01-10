#include "JsonDeserializer.h"

#include "Core/Node.h"
#include "Core/RootNode.h"
#include <fstream>
#include "Core/ConnectHandler.h"
#include "Core/PlugRegister.h"
#include "Core/PlugBase.h"

#include "Knob/KnobTypeDesc.h"
#include <QFileInfo>
#include <QDir>

using namespace nlohmann;

SWW_CORE_NAMESPACE_BEGIN
JsonDeserializer::JsonDeserializer():_depth(0),_flags(kNone)/*,_excutedCommandflags(kDoNothing)*/
{

}

bool JsonDeserializer::deserialize(const std::string& fileName , Sww::Core::RootNode* rootNode){
    std::ifstream i(fileName);
    SwwJson j;
    i >> j;

    SwwJson jProj = j["ds_project"];

    if(!jProj.is_null()){

        if(!jProj["version"].is_null()){
            std::string version = jProj["version"];
            SWW_INF(version);

        }else{
            SWW_ERR("version declaration not found.");
            return false;
        }

        std::vector<int> frameRange;
        SwwJson jFrameRange = jProj["frameRange"];
        if(!jFrameRange.is_null()){
//            frameRange = jFrameRange;
            frameRange = jFrameRange.get<std::vector<int>>();
            SWW_DEBUG_VALUE(frameRange[0] << "," << frameRange[1]);
        }else{
            SWW_ERR("frame range not found.");
            return false;
        }

        if(!frameRange.empty())
        {
            rootNode->startFrame(frameRange[0]);
            rootNode->endFrame(frameRange[1]);
            rootNode->setGlobalFrame(frameRange[0]);
            rootNode->emitframeRangeChanged();
        }

        SwwJson jRoot = jProj["$root"];
        if(!jRoot.is_null()){
            Imath::Box2f dummy;
            if(!deserialize(jRoot,(Node*)rootNode,dummy))return false;

        }else{
            SWW_ERR("empty root.");
        }

    }
    else{
        SWW_ERR("not ds");
    }

    return true;
}

bool JsonDeserializer::deserialize(SwwJson &j, Sww::Core::Node *node,
                                   Imath::Box2f& area) {

    if(!j.is_null()){

        runCommand(j,node);//コマンドがなければスルー
        if(_flags & kJustRunCommand)return true;

        if(_depth == 0){
            if(_flags & kSolveNameConflict){
                if(!solveNameConflict(j,node)){
                    SWW_ERR("parse failed. " << node->getName());
                    return false;
                }
            }
        }

        _depth++;

        for (SwwJson::const_iterator it = j.begin(); it != j.end(); ++it) {

            const std::string nodeName = it.key();
            SwwJson jNode = it.value();

            if(!jNode.is_null()){

                SwwJson jProperty = jNode["%pr"];

                if(jProperty.is_null()){
                    SWW_ERR("property is null.");
                    _depth--;
                    return false;
                }

                SwwJson jOW = jProperty["%ow"];
                SwwJson jType = jProperty["type"];
                SwwJson jPos  = jProperty["pos"];
                SwwJson jConn = jProperty["conn"];
                SwwJson jValue = jProperty["value"];
                SwwJson jDynamicValue = jProperty["dValue"];
                SwwJson jUdd  = jProperty["udd"];//user defined dynamic plug.

                bool overWriteMode = false;
                if(!jOW.is_null()){
                    overWriteMode = jOW;
                    //if(overWriteMode)_flags |= kOverWriteMode;
                }

                Sww::Core::Node* nodePtr;
                if(overWriteMode){
                    nodePtr =  node->_children.value(nodeName).get();
                    if(!nodePtr){
                        SWW_ERR("node not found. : " << nodeName);
                        _depth--;
                        return false;
                    }
                    if(_depth == 1){
                        _updatedNodes.push_back(nodePtr);
                    }
                }else{
                    std::string typeValue;
                    if(!jType.is_null())typeValue = jType;
                    if(!node->createNode(typeValue,nodePtr,nodeName))
                    {
                        SWW_ERR("createNode failed." << typeValue << ":" << nodeName);
                        _depth--;
                        return false;
                    }
                }

                //set udd.
                if(!jUdd.is_null()){
                    for (SwwJson::const_iterator it0 = jUdd.begin(); it0 != jUdd.end(); ++it0) {
                        const std::string& plugName = it0.key();
                        SwwJson jUddValue = it0.value();
                        SwwJson jPlugType = jUddValue["type"];
                        SwwJson jKnobType = jUddValue["knobType"];
                        SwwJson jOutput = jUddValue["output"];
                        SwwJson jConnectable = jUddValue["connectable"];
                        SwwJson jStorable = jUddValue["storable"];
                        SwwJson jHidden = jUddValue["hidden"];
                        SwwJson jMultiConnect = jUddValue["multiConnect"];

                        std::string plugType;
                        SwwKnobTypeDesc knobType = 0;
                        bool output = false;
                        bool connectable = false;
                        bool storable = false;
                        bool hidden = true;
                        bool multiConnect = false;

                        if(!jPlugType.is_null())plugType = jPlugType;
                        if(!jKnobType.is_null())knobType = jKnobType;
                        if(!jOutput.is_null())output = jOutput;
                        if(!jConnectable.is_null())connectable = jConnectable;
                        if(!jStorable.is_null())storable = jStorable;
                        if(!jHidden.is_null())hidden = jHidden;
                        if(!jMultiConnect.is_null())multiConnect = jMultiConnect;

                        Sww::Core::PlugBase* pTmp = Sww::Core::PlugRegister::create(plugType,plugName);
                        if(!pTmp)continue;
                        pTmp->setKnobType(knobType);
                        pTmp->setOutput(output);
                        pTmp->setConnectable(connectable);
                        pTmp->setStorable(storable);
                        pTmp->setHidden(hidden);
                        pTmp->setMultiConnect(multiConnect);
                        pTmp->setUserDefined(true);

                        nodePtr->addDynamicPlug(pTmp);
                    }
                }

                //set pos.
                if(!jPos.is_null()){
                    std::vector<float> posValue;
//                    posValue = jPos;
                    posValue = jPos.get<std::vector<float>>();
                    Imath::V2f posVec(posValue[0],posValue[1]);
                    nodePtr->posInGraph(posVec);
                    area.extendBy(posVec);
                }

                //perse children.
                jNode.erase("%pr");
                if(!jNode.is_null()){
                    Imath::Box2f dummy;
                    if(!deserialize(jNode,nodePtr,dummy)){
                        SWW_ERR("parse children failed. " << nodePtr->getName());
                    }
                }

                //prepare connection.
                if(!jConn.is_null()){
                    std::vector<std::string> connArray = jConn;
                    for(std::vector<std::string>::iterator it0 = connArray.begin();
                        it0 != connArray.end();++it0){

                        QString connValueStrQt = (*it0).c_str();

                        //予約語処理　%this
                        connValueStrQt.replace("%this",nodeName.c_str());

                        QStringList connValues = connValueStrQt.split(" ");
                        std::string p0 = connValues.takeFirst().toStdString();


                        Q_FOREACH(QString valueStr,connValues){

                            QFileInfo inf(valueStr);
                            QString connNodeNameWithPath = inf.path() + "/" + inf.baseName();
                            QString connPlugName = inf.suffix();

                            Sww::Core::ConnectHandler* hnd;

                            std::string nO = connNodeNameWithPath.toStdString();
                            std::string pO = connPlugName.toStdString();
                            std::string nI = nodeName;
                            std::string pI = p0;

                            hnd = new Sww::Core::ConnectHandler(
                                        nO,
                                        pO,
                                        nI,
                                        pI,
                                        node);

                            node->add2ConnectHandler(hnd);
                        }
                    }//foreach conn values.
                    if(connArray.size())_delayedConncetTargets.push_back(node);
                }

                DelayedValue delayedValue;
                delayedValue.targetNode = nodePtr;

                //prepare set value.
                if(!jValue.is_null()){
                    std::vector<std::string> valueArray = jValue;
                    for(std::vector<std::string>::iterator it0 = valueArray.begin();
                        it0 != valueArray.end();++it0){
                        delayedValue.values.push_back((*it0));
                    }
                }

                //prepare set dynamic value.
                if(!jDynamicValue.is_null()){
                    std::vector<std::string> dynamicValueArray = jDynamicValue;
                    for(std::vector<std::string>::iterator it0 = dynamicValueArray.begin();
                        it0 != dynamicValueArray.end();++it0){
                        delayedValue.dynamicValues.push_back((*it0));
                    }
                }

                _delayedValues.push_back(delayedValue);

            }// if jNode is null.
        }// forearch jChildren.

        _depth--;

        if(_depth==0){
            if(!delayedConstruct())return false;
            for(auto n:_updatedNodes){
                n->emitNodeStateChanged();
            }
            _updatedNodes.clear();

        }
    }

    return true;
}

bool JsonDeserializer::delayedConstruct(){

    //1,delayed set value.
    for(std::vector<DelayedValue>::iterator it0 = _delayedValues.begin(); it0 != _delayedValues.end();++it0){
        DelayedValue& v = (*it0);
        v.targetNode->_inSetValuePhase = true;
        for(std::vector<std::string>::iterator it1 = v.values.begin();
            it1 !=  v.values.end();++it1){
            std::string& str = (*it1);
            QStringList valueElement =QString(str.c_str()).split(" ");

            if(!v.targetNode->setValueString(valueElement[0],valueElement[1])){
                SWW_ERR("set value failed. [" << valueElement[0].toStdString()<< ":" << valueElement[1].toStdString() << "]");
            }
        }
        v.targetNode->_inSetValuePhase = false;
    }

    //2,pre initial connect.
    for(std::vector<DelayedValue>::iterator it0 = _delayedValues.begin(); it0 != _delayedValues.end();++it0){
        DelayedValue& v = (*it0);
        v.targetNode->_inPreInitialConnectPhase = true;
        if(!v.targetNode->preInitialConnect()){
            SWW_ERR("preInitialConnect faield. " << v.targetNode->getName());
        }
        v.targetNode->_inPreInitialConnectPhase = false;
    }

    //3,delayed connect.
    for(std::vector<Sww::Core::Node*>::iterator it0 = _delayedConncetTargets.begin(); it0 != _delayedConncetTargets.end();++it0){
        Sww::Core::Node* n = (*it0);
        n->_inConnectPhase = true;
        if(!n->connectByHandler()){
            SWW_ERR("connect failed. " << n->getName());
        }
        n->_inConnectPhase = false;
    }

    //4,delayed set dynamic value.
    for(std::vector<DelayedValue>::iterator it0 = _delayedValues.begin(); it0 != _delayedValues.end();++it0){
        DelayedValue& v = (*it0);
        v.targetNode->_inSetDynamicValuePhase = true;
        for(std::vector<std::string>::iterator it1 = v.dynamicValues.begin();
            it1 !=  v.dynamicValues.end();++it1){
            std::string& str = (*it1);
            QStringList valueElement =QString(str.c_str()).split(" ");

            if(!v.targetNode->setValueString(valueElement[0],valueElement[1])){
                SWW_ERR("set value failed. [" << valueElement[0].toStdString()<< ":" << valueElement[1].toStdString() << "]");
            }
        }
        v.targetNode->_inSetDynamicValuePhase = false;
    }

    return true;
}

bool JsonDeserializer::solveNameConflict(SwwJson &j, Node *node){

    if(!j.is_null()){
        SwwJson newJ;
        for (SwwJson::iterator it = j.begin(); it != j.end(); ++it) {
            std::string nodeNameTmp = it.key();
            //SWW_DEBUG_VALUE(nodeNameTmp);
            std::string nodeName = node->genUniqueNodeName(nodeNameTmp);//ノードが既にあったら改名

            SwwJson& jNode = it.value();

            if(!jNode.is_null()){

                //solveReservedWord(jNode);// recurcive children.

                SwwJson jProperty = jNode["%pr"];

                if(jProperty.is_null()){
                    SWW_ERR("property is null. : " << nodeNameTmp);
                    return false;
                }

                SwwJson& jPos  = jProperty["pos"];
                std::vector<float> posValue;
                if(!jPos.is_null()){
//                    posValue = jPos;
                    posValue = jPos.get<std::vector<float>>();
                }
                else{
                    SWW_ERR("posValue is missing.");
                    return false;
                }
                posValue[0] += 50;
                posValue[1] += 50;
                jProperty["pos"] = posValue;

                SwwJson& jConn = jProperty["conn"];
                std::vector<std::string> connArray;
                if(!jConn.is_null())connArray = jConn;


                std::vector<std::string> connArrayNew;
                for(std::vector<std::string>::iterator it0 = connArray.begin();
                    it0 != connArray.end();++it0){

                    QStringList connValues = QString((*it0).c_str()).split(" ");
                    QString newConn = connValues.takeFirst();

                    Q_FOREACH(QString valueStr,connValues){

                        QFileInfo inf(valueStr);
                        QString connNodeName = inf.baseName();
                        QString connPlugName = inf.suffix();

                        QString connNodeNameOld = connNodeName;
                        if(inf.path() == "."){//同一階層にいる場合、衝突回避
                            connNodeName = node->genUniqueNodeName(connNodeName.toStdString()).c_str();//ノードが既にあったら改名
                        }
                        connNodeName =    inf.path() + "/" + connNodeName;
                        connNodeNameOld = inf.path() + "/" + connNodeNameOld;
                        connNodeName.replace("%this",nodeName.c_str());
                        connNodeNameOld.replace("%this",nodeNameTmp.c_str());//名前解決前の名前で置換

                        QString jsonPointerPath = connNodeNameOld;
                        jsonPointerPath = "/" + jsonPointerPath;
                        jsonPointerPath = QDir::cleanPath(jsonPointerPath);

                        if(jsonPointerPath == "/"){
                            continue;//接続先が含まれていなかったら除外
                        }
                        else{
                            SwwJson jClone = j;//operator[]を使うと、存在しない要素が追加されてしまうのでコピーする　重いかも
                            if(jClone[SwwJson::json_pointer(jsonPointerPath.toStdString())].is_null()){
                                continue;//接続先が含まれていなかったら除外
                            }
                        }

                        //SWW_DEBUG_VALUE(j.dump(1));

                        ////ここでのgenUniqueNodeNameは問題あり
                        //QString newConnNodeNameRoot = node->_nodeGraph->genUniqueNodeName(connNodeNameRoot.toStdString()).c_str();//ノードが既にあったら改名
                        //connNodeNameRoot.replace("$this",);
                        //connNodeName.replace(connNodeNameRoot,newConnNodeNameRoot);
                        //connNodeName.replace("$this",nodeName.c_str());

                        newConn += " " + connNodeName + "." + connPlugName;

                    }

                    connArrayNew.push_back(newConn.toStdString());
                }//foreach conn values.


                jProperty["conn"] = connArrayNew;
                jNode["%pr"] = jProperty;
                newJ[nodeName] = jNode;
            }
        }//forearch node.

        j = newJ;
    }
    else{
        return false;
    }

    return true;
}

void JsonDeserializer::setFlags(JsonDeserializerFlags flag){
    _flags |= flag;
}

void JsonDeserializer::resetFlags(){
    _flags = kNone;
}

void JsonDeserializer::removeFlags(JsonDeserializerFlags flag){
    _flags &= ~flag;//フラグ解除 ビット反転して論理積
}

void JsonDeserializer::runCommand(SwwJson &j, Node *node){

    SwwJson jCmdRoot = j["%cmd"];
    if(jCmdRoot.is_null()){
        j.erase("%cmd");
        return;
    }

    for (SwwJson::iterator it = jCmdRoot.begin(); it != jCmdRoot.end(); ++it){
        std::string cmdKey = it.key();
        SwwJson& jCmd = it.value();

        if(cmdKey== "rm")
        {
            if(!jCmd.is_null()){
                std::vector<std::string> targetNodes;
                targetNodes = jCmd;

                for(auto nodeName:targetNodes){
                    Sww::Core::Node::Ptr nodePtr =  node->_children.value(nodeName);
                    if(nodePtr)nodePtr->emitRemoveMyself();
                }

            }
            break;
        }
        else if(cmdKey== "sel")
        {
            if(!jCmd.is_null()){
                std::vector<std::string> targetNodes;
                targetNodes = jCmd;
                for(auto nodeName:targetNodes){
                    Sww::Core::Node::Ptr nodePtr =  node->_children.value(nodeName);
                    if(nodePtr)nodePtr->emitSelectMyself();
                }
                //_excutedCommandflags |= kSelect;
            }
            break;
        }
        else if(cmdKey== "desel")
        {
            node->emitDeselectChildren();
            break;
        }
        else if(cmdKey== "tr")
        {
            if(!jCmd.is_null()){
                std::string path;
                path = jCmd;
            }
            //_excutedCommandflags |= kTransition;
            break;
        }

    }

    j.erase("%cmd");
}

SWW_CORE_NAMESPACE_END
