#ifndef NODEGRAPHSCENE_H
#define NODEGRAPHSCENE_H

#include <QGraphicsScene>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsItem>

#include  "Gui/NodeMaker.h"
//#include  "Core/NodeGraph.h"

#include "Gui/GuiAPI.h"

#include "Core/SwwMacro.h"
#include "Core/SwwJson.h"

#include <QMutex>


namespace Sww::Core{
class ConnectHandler;
class Node;
class PlugBase;
}

namespace SwwGui{
class Connector;
class PlugItem;
class NodeItem;
class MoveCommand;
class SelectCommand;

class SWWGUISHARED_EXPORT NodeGraphScene : public QGraphicsScene
{
    Q_OBJECT
public:

    friend class NodeGraphWidget;

    NodeGraphScene(Sww::Core::Node* node, QObject *parent = 0);
    virtual ~NodeGraphScene();

    bool connect(PlugItem* startPlug,PlugItem* endPlug);
    bool connect(const QString &nodeNameStart,const QString &plugNameStart,
                 const QString &nodeNameEnd,const QString &plugNameEnd);
    bool disconnect(Connector* c);
    void disconnectGui(Connector* c, bool withEmit = true);//GUIのみdisconnectする  nodeItemをdeleteするときは、emitしない
    bool disconnectInternal(Connector* c);
    bool removeNode(NodeItem* n);
    void reconnect(Connector* c, PlugItem *picked, bool changeStart = true);

    void add2ConnectHandler(Sww::Core::ConnectHandler *c,int index = 0);
    bool connectByHandler();

    Sww::Core::Node* node()const{return _node;}
    void setFrameRange(int start ,int end);
    void addNodeGraphItem(QGraphicsItem *item);
    void moveBy(float dx,float dy){_rootItem->moveBy(dx,dy);}

    QGraphicsItem* getRootItem()const{return _rootItem;}
    QPointF posFromRoot(QPointF scenePos);

    void setup();

    const std::string& getNodeName()const;

    void deleteSelectedNodes();
    void copyToClipBoard();
    void groupingNodes(QPointF mousePos);
    void storeSelectedNodesAndConnectors();
    void clearNodesAndConnectorsSelection();
    void createMod(const std::string &jsonStr, QPointF mousePos);
    void createMod(const std::string &jsonStr);//ペースト位置補正しない　solveNameConflictしない
    void emitNodeIsSelected();

    //NodeGraphScene* parentNgs();
    std::string getPath()const;

    QRectF getSceneRect()const;

protected:

public Q_SLOTS:
    bool runBackGround(Sww::Core::PlugBase* p);
    void reset();
    void selectNodeItems(const std::vector<std::string>& nodeNames);

    //void deselectAllItems();

Q_SIGNALS:
    void nodeIsSelected();

public Q_SLOTS:
    NodeItem* createNode(const QString &nodeType,const QPointF& pos = QPointF(0.0f,0.0f),const QString &nodeName = QString());
    void setSelectionChangeFlag();

private:
    bool getNodeItem(const QString &nodeName,NodeItem*& nodeItemPtr);
    Sww::Core::Node* _node;//parent node

    //gui操作用コンテナ**********************************************************************************
    QHash<QString,NodeItem*> _nodeNamesContainer; //名前でnodeItemを取得するためのコンテナ
    QList<SwwGui::NodeItem*> _selectedNodeItems; //選択中ノード
    QList<SwwGui::Connector*> _selectedConnectors;//選択中コネクタ
    //**********************************************************************************

    bool connectInternal(PlugItem* startPlug,PlugItem* endPlug);  //p1 = output, p2 = input
    void connectGui(PlugItem* startPlug,PlugItem* endPlug);

    void prepareMoveCommand();
    void pushMoveCommand();

    void prepareSelectCommand();
    void pushSelectCommand();

    QGraphicsItem* _rootItem;


    //SwwJson _currentSelection;

    MoveCommand* _moveCommand;
    SelectCommand* _selectCommand;
    QMutex _mutex;
    bool _waitForPushSelectCommand;
    bool _selectionChanged;
    bool _waitForPushMoveCommand;

    SWW_SIMPLE_MEMBER(bool,nodeItemIsMoved)
    //SWW_SIMPLE_MEMBER_READ_ONLY(NodeGraphScene*,parentNgs)
};
}

#endif // NODEGRAPHSCENE_H
