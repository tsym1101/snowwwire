//#include "NodeGraph.h"
//#include "DsMacro.h"

//#include "DsCore/NodeRegister.h"
//#include "PlugBase.h"
//#include "ConnectHandler.h"

//#include "PlugType.h"
//#include "Node.h"
//#include "DsCore/VectorHelper.h"

//#include <iterator>     // std::back_inserter
//#include <algorithm>    // std::copy

//#define DS_DEFAULT_CURRENT_FRAME 0

//namespace DsCore{

//QMutex g_mutex;

//std::map<int,std::vector<ConnectHandler*> > NodeGraph::_indexedConnectHndList;

//DsCore::IntPlug* NodeGraph::_globalFramePlug = 0;


////カレントフレーム、フレームレンジはtime sliderとglobalPlugと整合性をとる
//NodeGraph::NodeGraph(Node *parentNodePtr):_globalTimePlugInput(0),_globalTimePlugOutput(0),
//    _currentFrame(DS_DEFAULT_CURRENT_FRAME),_startFrame(0),_endFrame(100),_level(0)
//{
//    bool valid = initialize();
//    Q_ASSERT(valid);
//}

//NodeGraph::~NodeGraph()
//{
////    std::map<std::string, Node*>::iterator it = _nodeTable.begin();
////    while( it != _nodeTable.end() )
////    {
////        Node* node = it->second;
////        //DS_DEBUG_VALUE(node->getName());
////        std::string name = node->getName();
////        delete node;
////        ++it;
////    }

//    _nodeTable.clear();
//    //_children.clear();
//}

//bool NodeGraph::initialize()
//{
//    DS_CHECK_BOOL(initGlobalNode()); //globalNodeを作成し、plugを保持
//    return true;
//}

//bool NodeGraph::initGlobalNode()
//{
//    Node* globalNode;
//    DS_CHECK_BOOL(createNode("Global",globalNode));
//    DS_CHECK_BOOL(globalNode->getInputPlug(0,(PlugBase*&)_globalTimePlugInput));
//    DS_CHECK_BOOL(globalNode->getOutputPlug(0,(PlugBase*&)_globalTimePlugOutput));
//    return true;
//}


//bool NodeGraph::createNode(const std::string &nodeType)
//{
//    Node* node;
//    return createNode(nodeType,node);
//}

//bool NodeGraph::createNode(const std::string &nodeType, Node *&nodePtr,const std::string &name)
//{
//    bool valid;
//    valid = NodeRegister::create(nodeType,nodePtr,this);

//    if(!valid)
//    {
//        DS_ERR(nodeType << ": get creatorFunction failed.");
//        nodePtr = 0;    //失敗したらポインタにnullを代入
//        return valid;
//    }

//    NodePtr node(nodePtr);

//    //ノード命名**********************************************************
//    bool isContain = true;
//    int i = 1;
//    if(name.empty())//ノード名を指定せずに生成した時
//    {
//        while(1)    //名前がダブらずに追加できるまで
//        {
//            //iをstringに変換
//            std::string myName = nodeType + QString::number(i).toStdString();
//            isContain = mapContains(_nodeTable,myName);
//            if(!isContain)
//            {
//                node->setName(myName);
//                _nodeTable[myName] = node;
//                break;
//            }
//            i++;
//        }
//    }else//ノード名を指定して生成した時
//    {
//        isContain = mapContains(_nodeTable,name);
//        if(!isContain)
//        {
//            node->setName(name);
//            _nodeTable[name] = node;
//        }else{
//            while(1)    //名前がダブらずに追加できるまで
//            {
//                std::string myName = name + std::string("_") + QString::number(i).toStdString();
//                isContain = mapContains(_nodeTable,myName);
//                if(!isContain)
//                {
//                    node->setName(myName);
//                    _nodeTable[myName] = node;
//                    break;
//                }
//                i++;
//            }
//        }

//    }
//    //**********************************************************

//    return true;
//}

////void NodeGraph::removeNode(Node *node)
////{
////    _nodeTable.erase(node->getName()); //ノードテーブルから削除
////    delete node;    //解放
////}

//bool NodeGraph::removeNode(std::string nodeName)
//{
//    NodePtr node = _nodeTable[nodeName];

//    if(!node)
//    {
//        DS_ERR(nodeName << " doesn't exist.");
//        return false;
//    }

//    _nodeTable.erase(nodeName);

//    //removeNode(node);
//    return true;
//}

////bool NodeGraph::getNode(std::string nodeName,Node *&node)
////{
////    node = _nodeTable[nodeName];
////    if(!node)
////    {
////        DS_ERR(nodeName << " doesn't exist.");
////        return false;
////    }
////    return true;
////}

//bool NodeGraph::connect(PlugBase* p1,PlugBase* p2)//p1 = output, p2 = input
//{

//    //DS_DEBUG("connect 1 *************");

//    bool stat;

//    if(p1->isOutput() && !p2->isOutput()){ //通常の接続
//        stat = p1->connect(p2);
//        if(!stat) {
//            DS_ERR("connect failed. " << p1->getDebugName() << "->" << p2->getDebugName());
//            return false;
//        }

//        stat = p2->postSetRoutine();

//        if(!stat)
//        {
//            DS_ERR("postSetRoutine failed." << p2->getDebugName());
//            disconnect(p1,p2);
//            return false;
//        }
//    }
//    else if(!p1->isOutput() && !p2->isOutput()){
//        stat = p2->illegalConnect(p1);
//        if(!stat) {
//            DS_ERR("connect failed. " << p1->getDebugName() << "->" << p2->getDebugName());
//            return false;
//        }
//        p2->pushDirty();

//    }
//    else if(p1->isOutput() && p2->isOutput()){
//        stat = p1->illegalConnect(p2);
//        if(!stat) {
//            DS_ERR("connect failed. " << p1->getDebugName() << "->" << p2->getDebugName());
//            return false;
//        }
//        p2->pushDirty();
//    }
//    else{
//        Q_ASSERT(0);
//    }

//    return true;
//}

//bool NodeGraph::connect(NodePtr& outputNode, int outputIndex, NodePtr& inputNode, int inputIndex)
//{
//    //DS_DEBUG("connect 2 *************");

//    PlugBase* outputPlug = 0;
//    PlugBase* inputPlug = 0;

//    //接続ノードをインデックスで取得
//    bool stat = outputNode->getOutputPlug(outputIndex,outputPlug);
//    if(!stat) return false;
//    stat = inputNode->getInputPlug(inputIndex,inputPlug);
//    if(!stat) return false;

//    return connect(outputPlug,inputPlug);
//}

//bool NodeGraph::connect(std::string outputNodeName, int outputIndex, std::string inputNodeName, int inputIndex)
//{
//    //DS_DEBUG("connect 3 *************");

//    bool stat;

//    Node* outputNode;
//    Node* inputNode;

//    //接続ノードを名前で取得
////    stat = getNode(outputNodeName,outputNode);
////    if(!stat) return false;
////    stat = getNode(inputNodeName,inputNode);
////    if(!stat) return false;

//    NodePtr& outputNodePtr = _nodeTable[outputNodeName];
//    if(!outputNode) return false;
//    NodePtr& inputNodePtr = _nodeTable[inputNodeName];
//    if(!inputNode) return false;

//    return connect(outputNodePtr,outputIndex,inputNodePtr,inputIndex);
//}

//bool NodeGraph::connect(std::string outputNodeName, const std::string &outputPlugName, std::string inputNodeName, const std::string &inputPlugName)
//{
//    //DS_DEBUG("connect 4 *************");

//    bool stat;

////    Node* outputNode;
////    Node* inputNode;

//    //接続ノードを名前で取得
////    stat = getNode(outputNodeName,outputNode);
////    if(!stat) return false;
////    stat = getNode(inputNodeName,inputNode);
////    if(!stat) return false;

//    NodePtr& outputNodePtr = _nodeTable[outputNodeName];
//    if(!outputNodePtr) return false;
//    NodePtr& inputNodePtr = _nodeTable[inputNodeName];
//    if(!inputNodePtr) return false;

//    PlugBase* outputPlug = 0;
//    PlugBase* inputPlug = 0;

//    //接続ノードを名前で取得
//    stat = outputNodePtr->getOutputPlug(outputPlugName,outputPlug);
//    if(!stat) {
//        stat = outputNodePtr->getOutputPlugDynamic(outputPlugName,outputPlug);
//        if(!stat) {
//            return false;
//        }
//    }
//    stat = inputNodePtr->getInputPlug(inputPlugName,inputPlug);
//    if(!stat){
//        stat = inputNodePtr->getInputPlugDynamic(inputPlugName,inputPlug);
//        if(!stat){
//            return false;
//        }
//    }

//    return connect(outputPlug,inputPlug);
//}

//bool NodeGraph::connect(PlugBase* outputPlug, NodePtr& inputNode, int inputIndex)
//{
//    //DS_DEBUG("connect 5 *************");

//    PlugBase* inputPlug = 0;

//    //接続ノードをインデックスで取得
//    bool stat = inputNode->getInputPlug(inputIndex,inputPlug);
//    if(!stat) return false;

//    return connect(outputPlug,inputPlug);
//}

//bool NodeGraph::disconnect(PlugBase *p1,PlugBase* p2) //p1 = output, p2 = input
//{


////    Q_ASSERT(p1->isOutput());
////    Q_ASSERT(!p2->isOutput());

//    p1->disconnect(p2);

//    if(p2->cloneWhenDisconnect() )p1->cloneDataTo(p2);
//    else p2->resetData();

//    bool stat = p2->postSetRoutine();
//    if(!stat){
//        DS_WARN("postSetRoutine failed after disconnect." << p2->getDebugName());
//    }

//    //DS_DEBUG("hogehoge");

//    return  true;
//}

//void NodeGraph::printAllNodes()
//{
//    std::string str("current nodes : ");

//    std::map<std::string, NodePtr>::iterator it = _nodeTable.begin();
//    while( it != _nodeTable.end() )
//    {
//        NodePtr& node = it->second;
//        str = str + node->getName() + ",";
//        ++it;
//    }

//    std::cout << str << std::endl;
//}

//void NodeGraph::printClean()
//{
//    std::cout << "clean state -----------------------------------" << std::endl;
//    std::map<std::string, NodePtr>::iterator it = _nodeTable.begin();
//    while( it != _nodeTable.end() )
//    {
//        NodePtr& node = it->second;
//        node->printCleanState();

//        ++it;
//    }
//}

//void NodeGraph::printAllInternalConnection()
//{
//    std::cout  << "All internal connections ..." << std::endl;
//    std::map<std::string, NodePtr>::iterator it = _nodeTable.begin();
//    while( it != _nodeTable.end() )
//    {
//        NodePtr& node = it->second;
//        node->printIntenalConnection();
//        ++it;
//    }
//}

//ConnectHandler* NodeGraph::createGlobalTimeConnectHandler(Node *n, int plugIndex)
//{
//    return new ConnectHandler(_globalTimePlugOutput,n,plugIndex,this);
//}

////void NodeGraph::setGlobalTime(int current)
////{
////    _currentFrame = current;
////    _globalTimePlugInput->setInputValue(current);
////}

////void NodeGraph::getGlobalTimeFromGlobalNode(int& current)
////{
////    DsCore::DsStatus stat;
////    stat = _globalTimePlug->getOutputValue(current);
////    if(stat != true) DS_ERR("getGlobalTime failed!!!!!!!!!!!!!!!!!!!!!!!");
////}

//bool NodeGraph::run(QObject* reciever, InfoToSubThread info)
//{
//    DsLog::setNoGuiMode();

//    *(info.stopped) = false;
//    info.end += 1;

//    for(int i = info.start;i<info.end;i++)
//    {
//        //        if(*(info.stopped))
//        //        {
//        //            DS_INF("render has canceled.");
//        //            break;
//        //        }

//        info.nodeGraph->setGlobalTime(i);
//        //bool result = info.node->execute(info.plug);
//        bool result = info.plug->pull();
//        if(!result){
//            DS_DEBUG("render failed. " << info.plug->getDebugName());
//            break;
//        }

//        int progress = (float(i - info.start) / float(info.end -info.start)) * 100;

//        //QMutexLocker locker(&g_mutex);

//        if(*(info.stopped)){
//            DS_INF("render has canceled.");
//            break;
//        }

//        //            DS_DEBUG("sleep start.");
//        //            Sleep(5000);
//        //            DS_DEBUG("sleep end.");

//        QMetaObject::invokeMethod(reciever,"setCurrentStatus",
//                                  Q_ARG(int,info.nodeGraph->currentFrame()),
//                                  Q_ARG(int,progress));
//    }//for loop i

//    info.nodeGraph->setGlobalTime(info.current);//timesliderとの整合性をとるため、カレントをもどす
//    info.nodeGraph->runFinished();
//    delete info.stopped;

//    DsLog::setGuiMode();

//    return true;
//}

//bool NodeGraph::run()
//{
//    std::vector<PlugBase*> runnerPlugs;


//    std::map<std::string, NodePtr>::iterator ite = _nodeTable.begin();
//    while (ite != _nodeTable.end()) {
//        NodePtr& n = ite->second;
//        if(n->isRunner())
//        {
//            std::copy(n->getRunnerPlugs().begin(),n->getRunnerPlugs().end(),std::back_inserter(runnerPlugs));//複製
//            //runnerPlugs += n->getRunnerPlugs();
//        }
//        ++ite;
//    }

//    for(std::vector<PlugBase*>::iterator it = runnerPlugs.begin();
//        it != runnerPlugs.end();
//        ++it)
//    {
//        PlugBase* p = *it;
//        DS_INF("run : " << p->getDebugName() );
//    }

//    for(int i = _startFrame;i<=_endFrame;i++)
//    {
//        setGlobalTime(i);
//        for( std::vector<PlugBase*>::iterator it = runnerPlugs.begin();
//             it != runnerPlugs.end();
//             ++it)
//        {
//            PlugBase* p = *it;
//            bool result =  p->pull();

//            //DS_DEBUG_VALUE(result);

//            if(!result)
//            {
//                DS_ERR("running process failed. exit. in " << p->getDebugName());
//                return result;
//            }
//        }

//        int progress = (float(i - _startFrame) / float(_endFrame - _startFrame)) * 100;

//        DS_INF("progress:" << progress << "%");
//    }

//    return true;
//}


//void NodeGraph::add2ConnectHandler(ConnectHandler *c,int index)
//{
//    _indexedConnectHndList[index].push_back(c);
//}

//bool NodeGraph::connectByHandler()
//{
//    //printAllNodes();

//    std::map<int,std::vector<ConnectHandler*> >::iterator it = _indexedConnectHndList.begin();
//    while( it != _indexedConnectHndList.end())
//    {
//        std::vector<ConnectHandler*> connectHndList = it->second;
//        for(std::vector<ConnectHandler*>::iterator i = connectHndList.begin(); i != connectHndList.end() ; ++i){
//            ConnectHandler* c = *i;
//            //DS_CHECK_BOOL(c->exec());
//            bool valid = c->exec();
//            if(!valid)DS_ERR("connect failed.");
//            delete c;
//        }
//        connectHndList.clear();
//        ++it;
//    }
//    _indexedConnectHndList.clear();

//    return true;
//}

////void NodeGraph::adaptFrameRange(bool adaptToStart)
////{
////    if(_startFrame > _endFrame)
////    {
////        if(adaptToStart)
////        {
////            _endFrame = _startFrame;
////        }else
////        {
////            _startFrame =_endFrame;
////        }
////    }

////    if(_currentFrame <  _startFrame)
////    {
////        //_startFrame = _currentFrame;
////        setGlobalTime( _startFrame);
////    }
////    if(_currentFrame > _endFrame)
////    {
////        //_endFrame = _currentFrame;
////        setGlobalTime(_endFrame);
////    }
////}

//}
