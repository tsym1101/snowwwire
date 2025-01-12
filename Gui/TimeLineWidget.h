#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <GL/glew.h>
#include <QtCore/QList>
#include <QtCore/QPointF>
#include <QtCore/QSize>
#include <QOpenGLWidget>
#include <QMutex>

#include "TextRenderer.h"
#include "TimeLineUtils.h"

namespace Sww::Core{
class TimeLine;
typedef std::shared_ptr<TimeLine> TimeLinePtr;
}

struct TimeLineZoomContext
{
    TimeLineZoomContext()
        : bottom(0.)
        , left(0.)
        , zoomFactor(1.)
    {
    }

    QPoint oldClick; /// the last click pressed, in widget coordinates [ (0,0) == top left corner ]
    double bottom; /// the bottom edge of orthographic projection
    double left; /// the left edge of the orthographic projection
    double zoomFactor; /// the zoom factor applied to the current image
};

class TimeLineWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit TimeLineWidget(QWidget* parent);

    void getBounds(SequenceTime* left, SequenceTime* right) const;
    void setTimeline(const Sww::Core::TimeLinePtr& timeline);
    Sww::Core::TimeLinePtr getTimeline() const;

    void seek(SequenceTime time);
    void renderText(double x,
                    double y,
                    const QString & text,
                    const QColor & color,
                    const QFont & font,
                    int flags) const;

public Q_SLOTS:
    void centerOn(SequenceTime left,
                  SequenceTime right,
                  int margin = 5);
    void onFrameChanged(SequenceTime, int);
    void recenterOnBounds();
    void setBoundaries(SequenceTime first, SequenceTime last);
    bool isFrameRangeEdited() const;
    void setFrameRangeEdited(bool edited);

signals:
    void boundariesChanged(SequenceTime, SequenceTime);
private:
    void setBoundariesInternal(SequenceTime first, SequenceTime last, bool emitSignal);

    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    virtual void paintGL();
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void wheelEvent(QWheelEvent* e);
    virtual void enterEvent(QEvent* e);
    virtual void leaveEvent(QEvent* e);

    virtual QSize sizeHint() const;



    double toTimeLine(double x) const;
    QPointF toTimeLineCoordinates(double x,double y) const;

    QPointF toWidgetCoordinates(double x, double y) const;
    double toWidget(double t) const;

    std::unique_ptr<QFont> _textFont;
    TimeLineZoomContext _tlZoomCtx;
    double _screenPixelRatio{1.0};
    TextRenderer _textRenderer;
    bool _firstPaint{true};
    bool _isTimeFormatFrames{true};
    bool _alphaCursor{false}; ///< should cursor be drawn semi-transparent
    QPoint _lastMouseEventWidgetCoord;
    mutable QMutex _boundariesMutex;
    SequenceTime _leftBoundary{0},_rightBoundary{0};
    bool _isFrameRangeEdited{false};
    mutable QMutex _frameRangeEditedMutex;
    Sww::Core::TimeLinePtr _timeline;
    bool _seekingTimeline{false};
    int _mousePressX{0}; ///< widget X coordinate of last click
    int _mouseMoveX{0}; ///< widget X coordinate of last mousemove position
    TimelineStateEnum _state{eTimelineStateIdle};
};

#endif // TIMELINEWIDGET_H
