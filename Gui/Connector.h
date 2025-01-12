#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "Gui/GuiAPI.h"
#include <QGraphicsItem>

namespace SwwGui{
class PlugItem;
class NodeGraphScene;

class SWWGUISHARED_EXPORT Connector : public QGraphicsItem
{
public:
    Connector(PlugItem* _startPlug,PlugItem* endPlug);
    virtual ~Connector();
    enum { Type = UserType + 3 };
    int type() const { return Type; }
    void updateLine();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void pushDirty();       ///< 接続先をdirtyにする。接続時、接続解除時に走らせる
    PlugItem* startPlug()const {return _startPlug;}
    PlugItem* endPlug()const {return _endPlug;}
    void startPlug(PlugItem* p) {_startPlug = p;}
    void endPlug(PlugItem* p) {_endPlug = p;}
    bool hasPlugItem(PlugItem* p){return (_startPlug == p || _endPlug == p );}

    std::pair<QPointF, QPointF> pointsC1C2() const; //calculate c1 & c2 control points.

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    enum Mode {PULL_STARTPOINT,PULL_ENDPOINT,DO_NOTHING};
    PlugItem *_startPlug, *_endPlug;
    QPointF startPoint,endPoint;
    Mode _myMode;
    NodeGraphScene* nodeGraphScene(){return (NodeGraphScene*)(scene());}
    int _lineWidth;
};
}

#endif // CONNECTOR_H
