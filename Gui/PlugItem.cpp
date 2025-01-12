#include "PlugItem.h"
#include "NodeItem.h"

//#include "_Types/ISwwType.h"

#include "Core/NodeGraph.h"
#include "Core/Plug.h"
#include "Core/Node.h"
#include "Core/RootNode.h"
#include "Gui/Connector.h"
#include "Gui/NodeGraphScene.h"

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif

#include <QGraphicsView>
#include "Core/PlugRegister.h"
#include "Core/ConnectHandler.h"

#include "Gui/ConnectDialog.h"

using namespace SwwGui;

PlugItem::PlugItem(Sww::Core::PlugBase *plug, NodeItem *parentNodeItem, QGraphicsItem* parent):
    QGraphicsItem(parent),_plug(plug),line(0),_isOutput(plug->isOutput())
  ,_parentNodeItem(parentNodeItem),_connector(0),_plugItemPassthrough(0),_contextMenu(0)
{
    setFlag(ItemIsSelectable);
    setCacheMode(DeviceCoordinateCache);    //キャッシュモード

    if(plug->isDynamic()){
        _myPen.setColor(Qt::darkGray);
        _myPen.setWidth(3);
    }
    else{
        //        _myPen.setColor(Qt::black);
        //        _myPen.setWidth(2);
        _myPen.setStyle(Qt::NoPen);
    }

    makeSphereColor();

    //passthrough plug
    if(!plug->isOutput() && plug->isPassthrough()
            && qgraphicsitem_cast<NodeItem*>(parent) ){
        _plugItemPassthrough = new PlugItem(plug,parentNodeItem,this);
        _plugItemPassthrough->setX(parentNodeItem->getNodeWidth());
        _plugItemPassthrough->setOutput(true);
    }else{
        _plugItemPassthrough = 0;
    }

    //createContextMenu();
}

PlugItem::~PlugItem()
{

}


void PlugItem::makeSphereColor()
{
    using namespace Sww::Core;

    QColor color0 ,colorSunken0;

    PlugColor pColor=  _plug->getPlugColor();

    color0.setRgb(pColor.r(),pColor.g(),pColor.b());
    //color0 = color0.dark(180);
    colorSunken0 = color0.light(180);

    _sphereBrush = QBrush(color0);
    _sphereBrushSunkun = QBrush(colorSunken0);
}

QRectF PlugItem::boundingRect() const
{
    return QRectF(-12, -12, 24, 24);
}

void PlugItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing);
    if (option->state & QStyle::State_Sunken) {
        //painter->setBrush(_gradientSunken);
        painter->setBrush(_sphereBrushSunkun);
    } else {
        //painter->setBrush(_gradient);
        painter->setBrush(_sphereBrush);
    }


    if(_plug->isClean()){
        _myPen.setStyle(Qt::NoPen);
    }
    else{
        _myPen.setStyle(Qt::SolidLine);
        _myPen.setColor(Qt::black);
        _myPen.setWidth(5);
    }

    if(_plug->isExternalConnection()){
        _myPen.setStyle(Qt::SolidLine);
        _myPen.setColor(Qt::gray);
        _myPen.setWidth(5);
    }

    painter->setPen(_myPen);
    painter->drawEllipse(-10, -10, 20, 20);

}

void PlugItem::emitChangeConnectState(bool isConnect)
{
    emit changeConnectState(isConnect);
}

void PlugItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton){
        //event->accept();
        QGraphicsItem::mousePressEvent(event);
        return;
    }

    if(!_plug->isFull())
    {
        QPointF pos = nodeGraphScene()->posFromRoot(event->scenePos());
        line = new QGraphicsLineItem(QLineF(pos,pos));
        line->setPen(QPen(Qt::white,4));
        line->setZValue(-1000);
        nodeGraphScene()->addNodeGraphItem(line);
    }

    update();
    QGraphicsItem::mousePressEvent(event);
}
void PlugItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!_plug->isFull())
    {
        if(line)
        {
            QLineF lineTmp(line->line().p1(),
                           nodeGraphScene()->posFromRoot(event->scenePos()));
            line->setLine(lineTmp);
        }
    }

    QGraphicsItem::mouseMoveEvent(event);
}

void PlugItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(!_plug->isFull())
    {
        QPointF offset = nodeGraphScene()->getRootItem()->pos();
        QList<QGraphicsItem *> pickedItems = scene()->items(line->line().p2() + offset); //lineのp2と交差しているitemがリストに入る
        if (pickedItems.count() && pickedItems.first() == line)       //lineそのものもリストに入ってしまうので消去する
            pickedItems.removeFirst();

        scene()->removeItem(line);
        delete line;

        if(pickedItems.empty())
        {
            QGraphicsItem::mouseReleaseEvent(event);
            return;
        }
        else if (pickedItems.first()->type() == PlugItem::Type)      //PlugItemだったら
        {
            PlugItem* pickedPlug = qgraphicsitem_cast<PlugItem *>(pickedItems.first());     //ダウンキャスト


            if(pickedPlug->_plug->isFull())    //複数接続不可かつ接続済みだったら
            {
                //接続解除をここに記述*********************************
                nodeGraphScene()->disconnect(pickedPlug->connector());
            }

            if(_isOutput != pickedPlug->isOutput()&&   //一方が入力、一方が出力だったら
                    pickedPlug->parentNodeItem() != parentNodeItem())   //親(NodeItem)が違ったら
            {
                if(_isOutput)
                {
                    //接続をここに記述************************************************
                    nodeGraphScene()->connect(this,pickedPlug);
                    //pickedPlug->prepareGeometryChange();
                    //pickedPlug->update();
                    QGraphicsItem::mouseReleaseEvent(event);
                    return;

                }else
                {
                    //接続をここに記述************************************************
                    nodeGraphScene()->connect(pickedPlug,this);
                    //this->update();
                    QGraphicsItem::mouseReleaseEvent(event);
                    return;

                }

                QGraphicsItem::mouseReleaseEvent(event);
                return;
            }
        }
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void PlugItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    if(!_contextMenu)createContextMenu();
    SWW_DEBUG("create context." << _plug->getDebugName());
    _contextMenu->exec(event->screenPos());
}

void PlugItem::createContextMenu()
{
    Q_ASSERT_X(!_parentNodeItem->nodeGraphScene()->views().isEmpty(),
               _plug->getDebugName().c_str(),"create context menu failed.");

    QWidget* parent = dynamic_cast<QWidget*>(_parentNodeItem->nodeGraphScene()->views()[0]);//ひとまずnodeGraphWidgetを親とする
    Q_ASSERT(parent);
    _contextMenu = new QMenu(parent);

    QAction *risePlugAction = new QAction("raise plug",_contextMenu);
    _contextMenu->addAction(risePlugAction);

    QAction *deleteUddPlugAction = new QAction("delete user defined plug",_contextMenu);
    _contextMenu->addAction(deleteUddPlugAction);

    QAction *connectByPahtAction = new QAction("connect by path",_contextMenu);
    _contextMenu->addAction(connectByPahtAction);

    QAction* printConnectionAction = new QAction("print connection",_contextMenu);
    _contextMenu->addAction(printConnectionAction);

    connect(risePlugAction,SIGNAL(triggered()),this,SLOT(raisePlug()));
    connect(deleteUddPlugAction,SIGNAL(triggered()),this,SLOT(deleteUddPlug()));
    connect(connectByPahtAction,SIGNAL(triggered()),this,SLOT(connectByPath()));
    connect(printConnectionAction,SIGNAL(triggered()),this,SLOT(printConnection()));
}

void PlugItem::raisePlug(){
    if(!_plug->raisePlug()){
        SWW_ERR("raise failed. " << _plug->getDebugName());
        return;
    }
    update();
}

void PlugItem::deleteUddPlug(){
    if(_plug->isUserDefined()){
        Sww::Core::NodeBase* nb = _plug->getNode();
        Sww::Core::Node* n  =  dynamic_cast<Sww::Core::Node*>(nb);
        Q_ASSERT(n);
        if(!n->deleteDynamicPlugItem(_plug->getName())){
            SWW_ERR("delete failed." << _plug->getDebugName());
        }
    }
    else{
        SWW_ERR("ONLY user defined plug is deletable." << _plug->getDebugName());
    }
}

void PlugItem::connectByPath(){
    QWidget* parent = dynamic_cast<QWidget*>(nodeGraphScene()->views()[0]);//ひとまずnodeGraphWidgetを親とする
    Q_ASSERT(parent);

    ConnectDialog* w = new ConnectDialog(_plug,parent);
    Qt::WindowFlags flags = w->windowFlags();
    w->setWindowFlags(flags|Qt::Window);
    w->show();
}

void PlugItem::printConnection(){
    _plug->printConnections();
}
