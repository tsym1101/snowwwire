#include "Connector.h"
#include "NodeItem.h"
#include "PlugItem.h"
#include "Core/NodeGraph.h"
#include "Gui/NodeGraphScene.h"
//#include <GL/glew.h>
#include <QPainterPathStroker>


using namespace SwwGui;

Connector::Connector(PlugItem* _startPlug,PlugItem* endPlug):_startPlug(_startPlug),_endPlug(endPlug),_lineWidth(4)
{
    setFlag(ItemIsSelectable);     //移動可能
    //setFlag((ItemIsFocusable));
    setCacheMode(DeviceCoordinateCache);    //キャッシュモード
    //setPen(QPen(Qt::black, 20, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    //updateLine();
}
Connector::~Connector()
{
}

void Connector::updateLine()
{
    prepareGeometryChange();
    startPoint = mapFromItem(_startPlug, 0, 0);
    endPoint = mapFromItem(_endPlug, 0, 0);
    //update();
}


QRectF Connector::boundingRect() const
{
    if (!_endPlug || !_startPlug)
        return QRectF();

    qreal extra = (2.0 + 40) / 2.0;

    QRectF basicRect = QRectF(startPoint, QSizeF(endPoint.x() - startPoint.x(),endPoint.y() - startPoint.y()))
            .normalized();

    auto points = pointsC1C2();
    QRectF c1c2Rect = QRectF(points.first, points.second).normalized();
    QRectF commonRect = basicRect.united(c1c2Rect);
    commonRect = commonRect.adjusted(-extra, -extra, extra, extra);

    return commonRect;
}

QPainterPath Connector::shape() const
{
    QPainterPath path;
    QPointF middlePoint = (endPoint - startPoint)/4;

    qreal startX,startY,endX,endY;
    startX = startPoint.x() + 8;
    startY = startPoint.y();
    endX = endPoint.x() - 8;
    endY = endPoint.y();

    path.moveTo(startX,startY);
    path.cubicTo(endX-middlePoint.x(),startY,startX + middlePoint.x(),endY,endX,endY);

    QPainterPathStroker stroker;
    stroker.setWidth(40);
    return stroker.createStroke(path);
}

void Connector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setClipRect(option->exposedRect);

    painter->setRenderHint(QPainter::Antialiasing);

    if(isSelected())
        painter->setPen(QPen(QColor(255,210,0),8));
    else{
        painter->setPen(QPen(Qt::lightGray, _lineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }


    QPainterPath path;

    qreal startX,startY,endX,endY;
    startX = startPoint.x() + 8;
    startY = startPoint.y();
    endX = endPoint.x() - 8;
    endY = endPoint.y();

    QPointF start = QPointF(startX,startY);
    QPointF end = QPointF(endX,endY);

    auto c1c2 = pointsC1C2();

    path.moveTo(start);
    path.cubicTo(c1c2.first,c1c2.second,end);
    painter->drawPath(path);
}

void Connector::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    QLineF lineDistance1(_startPlug->scenePos(),event->scenePos());
    qreal distance1 = lineDistance1.length();

    if(distance1 < 30)
    {
        startPoint = nodeGraphScene()->posFromRoot(event->scenePos());
        _myMode = PULL_STARTPOINT;
    }else{
        QLineF lineDistance2(_endPlug->scenePos(),event->scenePos());
        qreal distance2 = lineDistance2.length();
        if (distance2 < 30){

            endPoint = nodeGraphScene()->posFromRoot(event->scenePos());
            _myMode = PULL_ENDPOINT;
        }else
        {
            _myMode = DO_NOTHING;
        }
    }

    QGraphicsItem::mousePressEvent(event);
}

void Connector::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    prepareGeometryChange();
    switch (_myMode){
    case PULL_STARTPOINT:
    {
        startPoint = nodeGraphScene()->posFromRoot(event->scenePos());
        break;
    }

    case PULL_ENDPOINT:
        endPoint = nodeGraphScene()->posFromRoot(event->scenePos());
        break;
    }
    update();

    QGraphicsItem::mouseMoveEvent(event);
}

void Connector::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(_myMode == DO_NOTHING)
    {
        update();
        QGraphicsItem::mouseReleaseEvent(event);
        return;
    }

    QList<QGraphicsItem *> pickedItems = scene()->items(event->scenePos()); //lineのp2と交差しているitemがリストに入る
    if (pickedItems.count() && pickedItems.first() == this)       //lineそのものもリストに入ってしまうので消去する
        pickedItems.removeFirst();

    if(pickedItems.empty())     //何もないところで離したら
    {
        //接続解除をここに記述*******************************
        nodeGraphScene()->disconnect(this);
        return;
    }

    else if (pickedItems.first()->type() == PlugItem::Type)     //PlugItemだったら
    {
        PlugItem* pickedPlug = qgraphicsitem_cast<PlugItem *>(pickedItems.first());

        if(_startPlug == pickedPlug ||_endPlug == pickedPlug)    //接続中のポートだったら何もしない
        {
            updateLine();
            QGraphicsItem::mouseReleaseEvent(event);
            return;
        }

        switch (_myMode){
        case PULL_STARTPOINT:
            if(pickedPlug->isOutput())
            {
                nodeGraphScene()->reconnect(this,pickedPlug);
            }

            break;

        case PULL_ENDPOINT:

            if(! pickedPlug->isOutput())
            {
                nodeGraphScene()->reconnect(this,pickedPlug,false);
            }
            break;
        }
    }

    QGraphicsItem::mouseReleaseEvent(event);
}

std::pair<QPointF, QPointF> Connector::pointsC1C2() const{
    qreal startX,startY,endX,endY;
    startX = startPoint.x() + 8;
    startY = startPoint.y();
    endX = endPoint.x() - 8;
    endY = endPoint.y();

    QPointF start = QPointF(startX,startY);
    QPointF end = QPointF(endX,endY);

    const QPointF& out = start;
    const QPointF& in = end;


    const double defaultOffset = 200;

    double xDistance = in.x() - out.x();

    double horizontalOffset = qMin(defaultOffset, std::abs(xDistance));

    double verticalOffset = 0;

    double ratioX = 0.5;

    if (xDistance <= 0)
    {
      double yDistance = in.y() - out.y() + 20;

      double vector = yDistance < 0 ? -1.0 : 1.0;

      verticalOffset = qMin(defaultOffset, std::abs(yDistance)) * vector;

      ratioX = 1.0;
    }

    horizontalOffset *= ratioX;

    QPointF c1(out.x() + horizontalOffset,
               out.y() + verticalOffset);

    QPointF c2(in.x() - horizontalOffset,
               in.y() - verticalOffset);

    return std::make_pair(c1, c2);
}


