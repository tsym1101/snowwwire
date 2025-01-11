//#ifndef NODEGRAPH_H
//#define NODEGRAPH_H

//#include "DsCoreAPI.h"

//#include <QObject>
//#include <map>
//#include <sstream>
//#include <iostream>
//#include <vector>

//#include "DsCore/DsMacro.h"

//#include <QMutex>

//#include "DsTypeDef.h"

//namespace DsGui{
//class NodeGraphScene;
//}

//namespace DsCore{
//class Node;
//class PlugBase;
//class NodeGraph;
//class IntPlug;
//class ConnectHandler;


//struct DSCORESHARED_EXPORT InfoToSubThread{
//    int start;
//    int end;
//    int current;
//    DsCore::NodeGraph* nodeGraph;
//    DsCore::PlugBase* plug;
//    bool* stopped;
//};

//class DSCORESHARED_EXPORT NodeGraph : public QObject
//{
//    Q_OBJECT

//public:

//    friend class DsGui::NodeGraphScene;

//    NodeGraph();
//    ~NodeGraph();
//    bool initialize();  //ノードグラフ初期化

//    static bool connect(PlugBase* p1,PlugBase* p2);  //p1 = output, p2 = input
//    static bool connect(NodePtr &outputNode, int outputIndex , NodePtr &inputNode, int inputIndex);

//    bool connect(std::string outputNodeName,int outputIndex , std::string inputNodeName,int inputIndex);
//    bool connect(std::string outputNodeName,const std::string &outputPlugName , std::string inputNodeName,const std::string &inputPlugName);
//    bool connect(PlugBase* outputPlugBase, NodePtr &inputNode, int inputIndex);

//    static bool disconnect(PlugBase* p1,PlugBase* p2);//p1 = output, p2 = input

//    bool createNode(const std::string &nodeType); //ノードタイプ名によるノード生成
//    bool createNode(const std::string &nodeType,Node*& nodePtr, const std::string &name = std::string());

//    void printAllNodes();
//    void printClean();

//    void printAllInternalConnection();

//    //void removeNode(Node* node);
//    bool removeNode(std::string nodeName);

//    ConnectHandler*  createGlobalTimeConnectHandler(Node* n,int plugIndex);//nとglobalTimeを遅延接続するためのハンドルオブジェクトを返す


//    //************************************************************************************************************
////    void setGlobalTime(int current);
////    void adaptFrameRange(bool adaptToStart);
////    void stepForward(){setGlobalTime(_currentFrame + 1);}
////    bool connectToGlobalTime(PlugBase* plug){return connect((PlugBase*)_globalTimePlugOutput,plug);}

//    //************************************************************************************************************
//    void add2ConnectHandler(ConnectHandler *c,int index = 0);
//    bool connectByHandler();

////    //nodeを走査するため　saveに使う やむなし
////    std::map<std::string, NodePtr>::iterator begin(){return _nodeTable.begin();}
////    std::map<std::string, NodePtr>::iterator end(){return _nodeTable.end();}

//    static bool run(QObject *reciever,InfoToSubThread info);//BGスレッドで動作する
//    bool run();//メインスレッドで動作する。


//    static void setGlobalFramePlug(DsCore::IntPlug* p){_globalFramePlug = p;}

//public Q_SLOTS:

//    //************************************************************************************************************
//Q_SIGNALS:
//    void runFinished();
//    //************************************************************************************************************

//private:
//    unsigned int _level;

//    DsCore::Node* _parentNode;
//    std::map<std::string,NodePtr> _nodeTable;

//    static DsCore::IntPlug* _globalFramePlug;

//    static std::map<int,std::vector<ConnectHandler*> > _indexedConnectHndList;

//    //************************************************************************************************************
//    bool initGlobalNode();
//    IntPlug* _globalTimePlugInput;
//    IntPlug* _globalTimePlugOutput;

//    DS_SIMPLE_MEMBER_READ_ONLY(int,currentFrame)
//    DS_SIMPLE_MEMBER(int,startFrame)
//    DS_SIMPLE_MEMBER(int,endFrame)

//    //************************************************************************************************************
//};

//}

//#endif // NODEGRAPH_H
