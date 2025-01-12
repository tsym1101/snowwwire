#ifndef NODEGRAPHWIDGET_H
#define NODEGRAPHWIDGET_H

#include <GL/glew.h>

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#include <QtGui/QGraphicsView>
#else
#include <QtWidgets/QtWidgets>
#include <QtWidgets/QGraphicsView>
#endif

#include <QMenu>
#include <QtGui/QContextMenuEvent>
#include <math.h>

#include "Gui/GuiAPI.h"
#include <ImathVec.h>

namespace Sww::Core{
class RootNode;
}

namespace SwwGui{
class NodeItem;
class Connector;
class NodeGraphScene;
}
class KnobWidget;

class SWWGUISHARED_EXPORT NodeGraphWidget : public QGraphicsView
{
    Q_OBJECT

public:

    explicit NodeGraphWidget(KnobWidget* knobWidget,
                             QWidget *parent = 0);
    ~NodeGraphWidget();

    SwwGui::NodeGraphScene* nodeGraphScene(){return _currentScene;}
    void readDeepLayers();   
    void createNode(const QString &nodeType,
                    const QPointF &pos,
                    const QString &nodeName = QString());

    bool setScene(const std::string& path, bool withSetup = true);
    void setSceneWithStackCommand(const std::string& path, bool withSetup = true);
    void setRootNode(Sww::Core::RootNode* node);
    SwwGui::NodeGraphScene* getNodeGraphSceneByPath(const std::string& path);

Q_SIGNALS:


public Q_SLOTS:
    void updateKnobWidget();
    void selectIGLNodes();
    void createMod(const std::string& jsonStr);
    void postSelectNodes();

    void scaleUp(double step = 1.2);
    void scaleDown(double step = 1.2);

protected:

    void drawBackground(QPainter *painter, const QRectF &rect);
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void enterEvent(QEvent *);
    void wheelEvent(QWheelEvent *event);
    bool focusNextPrevChild(bool next){return false;}//タブ移動禁止 うまくいかない

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    SwwGui::NodeGraphScene* _currentScene;
    QMenu *_myContextMenu;
    KnobWidget* _knobWidget;
    QPointF _lastMousePos;
    bool _readyForMove;//move()用フラグ　中ボタンでtrueになる
    bool _readyForScale;
    double _lastMousePosX,_lastMousePosY;
    QMap<QString,QString> _metaText;
    QMap<std::string,SwwGui::NodeGraphScene*> _scenes;
    Sww::Core::RootNode* _rootNode;
    //float _scale;

    void createContextMenu();   //右クリックメニュー作成
    QPointF getMousePos();

    void setCurrentPathText(const QString& path);
};

#endif // NODEGRAPHWIDGET_H
