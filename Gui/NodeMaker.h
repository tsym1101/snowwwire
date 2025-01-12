#ifndef NODEMAKER_H
#define NODEMAKER_H

#include <QObject>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui/QGraphicsScene>
#else
#include <QtWidgets/QGraphicsScene>
#endif

#include "Gui/GuiAPI.h"

class NodeGraphWidget;

//namespace SwwGui{
//class NodeGraphScene;
//}

class SWWGUISHARED_EXPORT NodeMaker : public QObject
{
    Q_OBJECT
public:
    explicit NodeMaker(const QString &nodeName,
                       NodeGraphWidget *parent);
    static void setMousePos(const QPointF& pos){_mousePos = pos;}
Q_SIGNALS:

public Q_SLOTS:
    void createNode();
    void createMod();

private:
    QString _nodeName;
    NodeGraphWidget* _ngw;
    static QPointF _mousePos;

};

#endif // NODEMAKER_H
