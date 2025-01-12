#ifndef SWWNODEITEM_H
#define SWWNODEITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOption>
#include <QKeyEvent>
#include <QMenu>
#include <QtGui/QContextMenuEvent>
#include <QObject>
#include <QMutex>
#include <QVBoxLayout>
#include <QUndoCommand>

#include "Core/SwwAssert.h"
#include "Gui/GuiAPI.h"
#include "Core/SwwTypeDef.h"
#include "Core/JsonSerializer.h"
#include "Core/JsonDeserializer.h"

namespace Sww::Core{
class Node;
class PlugBase;
struct ConnectState;
}

class KnobObject;
class KnobGroupWidget;
class KnobBaseWidget;

namespace SwwGui{

class Connector;
class PlugItem;
class NodeGraphScene;
class MoveCommand;

class SWWGUISHARED_EXPORT NodeItem : public QObject ,public QGraphicsItem
{
    Q_OBJECT

public:
    friend class NodeGraphScene;

    NodeItem(Sww::Core::Node *node,NodeGraphScene* scene,QGraphicsItem* parent = 0);
    virtual ~NodeItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    enum {Type = UserType + 1};
    int type() const{ return Type; }

    Sww::Core::Node* node()const{return _node;}   
    QWidget* getKnob();

    void updateKnobs();//auto update後の処理
    void updateConnectors();
    int getNodeWidth()const{return _nodeWidth;}

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    NodeGraphScene* nodeGraphScene();

    QRectF sceneBoundingRectWithTitleItem()const;

    //static void initFont();//Qt5だとノード生成時のフォント初期化がもっさりするので、アプリ起動のタイミングで初期化する

public Q_SLOTS:
    void addDynamicPlugItem(Sww::Core::PlugBase* plugPtr);
    void deleteDynamicPlugItem(const Sww::Core::PlugMetaPtr& plugMeta);
    void connectPlugItem(const Sww::Core::ConnectState* con);
    void recieveErrorOccurred(bool occurred);
    void removeMyself();
    void selectMyself();
    void addUserDefinedDynamicPlug();

Q_SIGNALS:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private Q_SLOTS:
    void changeExcutingColor();
    void changeOrdinaryColor();
    void showInfo();
    void updateNodeItem();

private:
    QList<Connector*> _connectorList;
    QList<PlugItem*> _plugItemList;//input output 関係なく
    QHash<QString,PlugItem*> _plugNamesContainer; //名前でプラグアイテムを取得するためのコンテナ
    QList<PlugItem*> _connectablePlugItemListDynamic;//input output 関係なく plug位置補正に使用
    QList<KnobBaseWidget*> _visibleKnobs;
    int _staticConnectablePlugItemCount;

    Sww::Core::Node *_node;
    QString _myName;

    QPen _myPen;
    void setup();   //nodeからnodeItemを生成
    PlugItem *setPlugItem(Sww::Core::PlugBase *plugPtr, int current );//GUI配置のため、配置中のポートインデックスと総ポート数を渡す

    QMenu* _contextMenu;

    int _orgX,_orgY,_nodeWidth,_nodeHeight;
    int _plugNameOrgY,_plugNameHeight;

    NodeGraphScene* _scene;//自分がいるQGraphicsScene
    KnobGroupWidget* _knobGroup;

    QGraphicsTextItem* _titleItem; //ノード名表示

    //QMutex _mutex;

    //for NodeGraphScene ---------------------
    void preRemoveMyself();
    void addConnector(Connector *connector);
    void removeConnector(Connector* cnct);
    bool getPlugItem(const QString &plugName, PlugItem* &p);
    //----------------------------------------

    void addDynamicPlugItem(Sww::Core::PlugBase* plugPtr,int current);
    void createKnob(Sww::Core::PlugBase *plug);//1つのvisible knobを生成
    void removeKnob(const Sww::Core::PlugMetaPtr &plugMeta);//1つのvisible knoeを削除
    void createKnobGroup();//knob全体を作成
    void createContextMenu();   //右クリックメニュー作成

    //std::string _undoJson;
    NodeGraphScene* _childScene;

    //MoveCommand* _moveCommand;//move前の状態をメンバーに保持 pushCommandすることで解放

//    bool _positionChanged;
//    bool _clicked;//trueのときだけ_positionChangedのフラグがたつ

};
}

#endif // NODEITEM_H
