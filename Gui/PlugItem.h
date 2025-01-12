#ifndef PLUGITEM_H
#define PLUGITEM_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QObject>

//#include "_Types/SwwTypeDesc.h"

#include "Gui/GuiAPI.h"

namespace Sww::Core{
class PlugBase;
}

namespace SwwGui{

class Connector;
class NodeItem;
class NodeGraphScene;

class SWWGUISHARED_EXPORT PlugItem :public QObject, public QGraphicsItem
{
    Q_OBJECT

    friend class NodeGraphScene;

public:
    PlugItem(Sww::Core::PlugBase *plug, NodeItem* parentNodeItem, QGraphicsItem *parent); //親がPlugItemの場合もある
    virtual ~PlugItem();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    enum { Type = UserType + 2 };
    int type() const
    { return Type; }

    bool isOutput()const{ return _isOutput;}
    void setOutput(bool b){_isOutput = b;}
    NodeItem *parentNodeItem() const {return _parentNodeItem;}
    void connector(Connector *cnct){_connector = cnct;}
    Connector *connector() const {return _connector;}
    Sww::Core::PlugBase *plug() const {return _plug;}
    void emitChangeConnectState(bool isConnect);

    PlugItem* getplugItemPassthrough()const{return _plugItemPassthrough;}

signals:
    void changeConnectState(bool isConnect);

private Q_SLOTS:
    void raisePlug();
    void deleteUddPlug();
    void connectByPath();
    void printConnection();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
    enum _myMode{PULL_FROM_START,PULL_FROM_END,DO_NOTHING};
    QGraphicsLineItem *line;        //ポートから引っ張った時のラインを描画するために使う。一時領域
    bool _isOutput;
    NodeItem *_parentNodeItem;
    Connector* _connector;  //単数接続のポートを再接続する際、接続中のコネクターを削除するためだけに使う
    Sww::Core::PlugBase *_plug;
    QPen _myPen;

    QBrush _sphereBrush,_sphereBrushSunkun;
    void makeSphereColor();
    NodeGraphScene* nodeGraphScene(){return (NodeGraphScene*)scene();}
    void createContextMenu();

    PlugItem* _plugItemPassthrough;
    QMenu* _contextMenu;
};
}
#endif // PLUGITEM_H
