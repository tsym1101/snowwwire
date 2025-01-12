#include <cmath>
#include <iostream>
#include <QtGui/QMouseEvent>

#include "TimeLineWidget.h"
#include "TimeLine.h"


#define TICK_HEIGHT 7
#define CURSOR_WIDTH 15
#define CURSOR_HEIGHT 8

#define DEFAULT_TIMELINE_LEFT_BOUND 0
#define DEFAULT_TIMELINE_RIGHT_BOUND 100

#define USER_KEYFRAMES_HEIGHT 7


TimeLineWidget::TimeLineWidget(QWidget *parent)
    :QOpenGLWidget(parent)
{

}

void TimeLineWidget::initializeGL(){
    _textFont.reset(new QFont());
}

void TimeLineWidget::resizeGL(int width, int height){
    if (width == 0) {
        width = 1;
    }
    if (height == 0) {
        height = 1;
    }
    glViewport (0, 0, width, height);

    //    std::cout << "resize!!!" << std::endl;
}

void
TimeLineWidget::seek(SequenceTime time)
{
    if ( time != _timeline->currentFrame() ) {
        _seekingTimeline = true;
        _timeline->onFrameChanged(time);
        _seekingTimeline = false;
        update();
    }
}

void
TimeLineWidget::mousePressEvent(QMouseEvent* e)
{
    _mousePressX = e->x();
    _mouseMoveX = _mousePressX;
    if ( buttonDownIsMiddle(e) ) {
        _state = eTimelineStatePanning;
    } else if ( buttonDownIsRight(e) ) {
        _state = eTimelineStateSelectingZoomRange;
    } else {
        _lastMouseEventWidgetCoord = e->pos();
        const double t = toTimeLine(_mousePressX);
        SequenceTime tseq = std::floor(t + 0.5);
        if ( modCASIsControl(e) ) {
            int leftBound, rightBound;
            {
                QMutexLocker k(&_boundariesMutex);
                leftBound = _leftBoundary;
                rightBound = _rightBoundary;
            }
            _state = eTimelineStateDraggingBoundary;
            int firstPos = toWidget(leftBound - 1);
            int lastPos = toWidget(rightBound + 1);
            int distFromFirst = std::abs(e->x() - firstPos);
            int distFromLast = std::abs(e->x() - lastPos);
            if (distFromFirst  > distFromLast) {
                setBoundariesInternal(leftBound, tseq, true); // moving last frame anchor
            } else {
                setBoundariesInternal( tseq, rightBound, true );   // moving first frame anchor
            }
        } else {
            _state = eTimelineStateDraggingCursor;
            seek(tseq);
        }
    }
}

void
TimeLineWidget::mouseMoveEvent(QMouseEvent* e)
{
    int mouseMoveXprev = _mouseMoveX;

    _lastMouseEventWidgetCoord = e->pos();
    _mouseMoveX = e->x();
    const double t = toTimeLine(_mouseMoveX);
    SequenceTime tseq = std::floor(t + 0.5);
    bool distortViewPort = false;
    bool onEditingFinishedOnly = false;
    //    bool onEditingFinishedOnly = appPTR->getCurrentSettings()->getRenderOnEditingFinishedOnly();
    if (_state == eTimelineStatePanning) {
        _tlZoomCtx.left += toTimeLine(mouseMoveXprev) - toTimeLine(_mouseMoveX);
        update();
        //        if ( _gui->isTripleSyncEnabled() ) {
        //            _updateEditorFrameRanges();
        //            _updateOpenedViewersFrameRanges();
        //        } else if{
    }if (_state == eTimelineStateSelectingZoomRange) {
        // https://github.com/MrKepzie/Natron/issues/917
        update();
    } else if ( (_state == eTimelineStateDraggingCursor) && !onEditingFinishedOnly ) {
        if ( tseq != _timeline->currentFrame() ) {
            //            _gui->setDraftRenderEnabled(true);
            //            _gui->getApp()->setLastViewerUsingTimeline( _viewer->getNode() );
            _seekingTimeline = true;
            _timeline->onFrameChanged(tseq);
            _seekingTimeline = false;
        }
        distortViewPort = true;
        _alphaCursor = false;
    } else if (_state == eTimelineStateDraggingBoundary) {
        int leftBound, rightBound;
        {
            QMutexLocker k(&_boundariesMutex);
            leftBound = _leftBoundary;
            rightBound = _rightBoundary;
        }
        int firstPos = toWidget(leftBound - 1);
        int lastPos = toWidget(rightBound + 1);
        int distFromFirst = std::abs(e->x() - firstPos);
        int distFromLast = std::abs(e->x() - lastPos);
        if (distFromFirst  > distFromLast) { // moving last frame anchor
            if (leftBound <= tseq) {
                setBoundariesInternal(leftBound, tseq, true);
            }
        } else { // moving first frame anchor
            if (rightBound >= tseq) {
                setBoundariesInternal(tseq, rightBound, true);
            }
        }
        distortViewPort = true;
        _alphaCursor = false;
    } else {
        _alphaCursor = true;
    }

    if (distortViewPort) {
        double leftMost = toTimeLine(0);
        double rightMost = toTimeLine(width() - 1);
        double delta = (rightMost - leftMost) * 0.02;
        if (tseq < leftMost) {
            centerOn(leftMost - delta, rightMost - delta, 0);
        } else if (tseq > rightMost) {
            centerOn(leftMost + delta, rightMost + delta, 0);
        } else {
            update();
        }
    } else {
        update();
    }
} // TimeLineWidget::mouseMoveEvent

void
TimeLineWidget::enterEvent(QEvent* e)
{
    _alphaCursor = true;
    update();
    QOpenGLWidget::enterEvent(e);
}

void
TimeLineWidget::leaveEvent(QEvent* e)
{
    _alphaCursor = false;
    update();
    QOpenGLWidget::leaveEvent(e);
}

void
TimeLineWidget::mouseReleaseEvent(QMouseEvent* e)
{
    if (_state == eTimelineStateSelectingZoomRange) {
        // - if the last selected frame is the same as the first selected frame, zoom on the PROJECT range
        //   (NOT the playback range as in the following, and NOT adding margins as centerOn() does)
        // - if they are different, zoom on that range
        double t = toTimeLine( e->x() );
        int leftBound = std::floor(t + 0.5);
        int rightBound = std::floor(toTimeLine(_mousePressX) + 0.5);
        if (leftBound > rightBound) {
            std::swap(leftBound, rightBound);
        } else if (leftBound == rightBound) {
            //            if ( !_viewerTab->isFileDialogViewer() ) {
            //                double firstFrame, lastFrame;
            //                _gui->getApp()->getFrameRange(&firstFrame, &lastFrame);
            //                leftBound = std::floor(firstFrame + 0.5);
            //                rightBound = std::floor(lastFrame + 0.5);
            //            } else {
            //                _viewerTab->getTimelineBounds(&leftBound, &rightBound);
            //            }
            getBounds(&leftBound, &rightBound);
        }

        centerOn(leftBound, rightBound, 0);

        /*if ( _gui->isTripleSyncEnabled() ) {
                _updateEditorFrameRanges();
                _updateOpenedViewersFrameRanges();
            }else */
    } if (_state == eTimelineStateDraggingCursor) {
        bool wasScrubbing = false;
        //            if ( _gui->isDraftRenderEnabled() ) {
        //                _gui->setDraftRenderEnabled(false);
        //                wasScrubbing = true;
        //            }
        //            _gui->refreshAllPreviews();

        //            SettingsPtr settings = appPTR->getCurrentSettings();
        //            bool onEditingFinishedOnly = settings->getRenderOnEditingFinishedOnly();
        //            bool autoProxyEnabled = settings->isAutoProxyEnabled();
//                    bool onEditingFinishedOnly = false;

        //            if (onEditingFinishedOnly) {
        //                double t = toTimeLine( e->x() );
        //                SequenceTime tseq = std::floor(t + 0.5);
        //                if ( ( tseq != _timeline->currentFrame() ) ) {
        //                    _gui->getApp()->setLastViewerUsingTimeline( _viewer->getNode() );
        //                    _timeline->onFrameChanged(tseq);
        //                }
        //            } else if (autoProxyEnabled && wasScrubbing) {
        //                _gui->getApp()->renderAllViewers(true);
        //            }
    }

    _state = eTimelineStateIdle;
    QOpenGLWidget::mouseReleaseEvent(e);
} // TimeLineWidget::mouseReleaseEvent

void
TimeLineWidget::wheelEvent(QWheelEvent* e)
{
    if (e->angleDelta().x() != 0) {
        return;
    }
    const double scaleFactor = std::pow( 1.00152, e->angleDelta().y() );

    double newZoomFactor = _tlZoomCtx.zoomFactor * scaleFactor;
    if (newZoomFactor <= 0.01) { // 1 pixel for 100 frames
        newZoomFactor = 0.01;
    } else if (newZoomFactor > 100.) { // 100 pixels per frame seems reasonable, see also DopeSheetView::wheelEvent()
        newZoomFactor = 100.;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    QPointF zoomCenter = toTimeLineCoordinates( e->position().x(), e->position().y() );
#else
    QPointF zoomCenter = toTimeLineCoordinates( e->x(), e->y() );
#endif
    double zoomRatio =   _tlZoomCtx.zoomFactor / newZoomFactor;
    _tlZoomCtx.left = zoomCenter.x() - (zoomCenter.x() - _tlZoomCtx.left) * zoomRatio;
    _tlZoomCtx.bottom = zoomCenter.y() - (zoomCenter.y() - _tlZoomCtx.bottom) * zoomRatio;

    _tlZoomCtx.zoomFactor = newZoomFactor;

    update();

//    if ( _gui->isTripleSyncEnabled() ) {
//        _updateEditorFrameRanges();
//        _updateOpenedViewersFrameRanges();
//    }
}


QSize
TimeLineWidget::sizeHint() const
{
    return QSize( TO_DPIX(1000), TO_DPIY(45) );
}

double
TimeLineWidget::toTimeLine(double x) const
{
    double w = (double)width();
    double left = _tlZoomCtx.left;
    double right = left +  w / _tlZoomCtx.zoomFactor;

    return ( ( (right - left) * x ) / w ) + left;
}

QPointF
TimeLineWidget::toTimeLineCoordinates(double x,
                                      double y) const
{
    double h = (double)height();
    double bottom = _tlZoomCtx.bottom;
    double top =  bottom +  h / _tlZoomCtx.zoomFactor;

    return QPointF( toTimeLine(x), ( ( (bottom - top) * y ) / h ) + top );
}

void TimeLineWidget::paintGL(){

    {
        double screenPixelRatio = 1.0;
        if (screenPixelRatio != _screenPixelRatio) {
            _screenPixelRatio = screenPixelRatio;
            _textFont.reset(new QFont(QString::fromUtf8("Droid Sans"), 11 * screenPixelRatio));
        }
    }
    assert(_textFont);

    SequenceTime leftBound, rightBound;
    {
        QMutexLocker k(&_boundariesMutex);
        leftBound = _leftBoundary;
        rightBound = _rightBoundary;
    }
    const SequenceTime currentTime = _timeline->currentFrame();

    //初期化前のwidthが小さい問題を回避
    if(width() < 400){
        return;
    }
    else{
        if (_firstPaint) {
            _firstPaint = false;

            if ( (rightBound - leftBound) > 10000 ) {
                centerOn(currentTime - 100, currentTime + 100);
            } else if ( (rightBound - leftBound) < 50 ) {
                centerOn(currentTime - DEFAULT_TIMELINE_LEFT_BOUND, currentTime + DEFAULT_TIMELINE_RIGHT_BOUND);
            } else {
                centerOn(leftBound, rightBound);
            }
        }
    }

    double w = (double)width();
    double h = (double)height();
    if (_tlZoomCtx.zoomFactor <= 0) {
        return;
    }
    double bottom = _tlZoomCtx.bottom;
    double left = _tlZoomCtx.left;
    double top = bottom +  h / (double)_tlZoomCtx.zoomFactor;
    double right = left +  (w / (double)_tlZoomCtx.zoomFactor);
    double clearR, clearG, clearB;
    clearR = 0.0;
    clearG = 0.0;
    clearB = 0.0;

    //    std::cout << "parentWidget layout:"
    //              << parentWidget()->layout()
    //              << " width:"
    //              << width()
    //              <<" height: "
    //             << height()
    //             << "_tlZoomCtx.zoomFactor:"
    //             << _tlZoomCtx.zoomFactor
    //             << " _tlZoomCtx.left:"
    //             <<_tlZoomCtx.left
    //            <<std::endl;

    if ( (left == right) || (top == bottom) ) {
        glClearColor(clearR, clearG, clearB, 1.);
        glClear(GL_COLOR_BUFFER_BIT);
        return;
    }

    {
        GLProtectAttrib a(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_POLYGON_BIT | GL_LINE_BIT | GL_ENABLE_BIT | GL_HINT_BIT | GL_SCISSOR_BIT | GL_TRANSFORM_BIT);
        //GLProtectMatrix p(GL_PROJECTION); // no need to protect
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(left, right, bottom, top, 1, -1);
        //GLProtectMatrix m(GL_MODELVIEW); // no need to protect
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClearColor(clearR, clearG, clearB, 1.);
        glClear(GL_COLOR_BUFFER_BIT);

        QPointF btmLeft = toTimeLineCoordinates(0, height() - 1);
        QPointF topRight = toTimeLineCoordinates(width() - 1, 0);

        //        /// change the background color of the portion of the timeline where images are lying
        //        double firstFrame, lastFrame;
        //        if ( !_viewerTab->isFileDialogViewer() ) {
        //            _gui->getApp()->getFrameRange(&firstFrame, &lastFrame);
        //        } else {
        //            int f, l;
        //            _viewerTab->getTimelineBounds(&f, &l);
        //            firstFrame = (double)f;
        //            lastFrame = (double)l;
        //        }
        double firstFrame, lastFrame;
        int f, l;
        getBounds(&f,&l);
        firstFrame = (double)f;
        lastFrame = (double)l;

        //        std::cout << "firstframe : " << firstFrame << " lastflame : " << lastFrame << std::endl;

        //        firstFrame = 0;
        //        lastFrame = 100;

        double bgR, bgG, bgB;
        bgR = 0.19;
        bgG = 0.19;
        bgB = 0.19;

        glColor4f(bgR, bgG, bgB, 1.);
        glBegin(GL_POLYGON);
        glVertex2f( firstFrame, btmLeft.y() );
        glVertex2f( firstFrame, topRight.y() );
        glVertex2f( lastFrame, topRight.y() );
        glVertex2f( lastFrame, btmLeft.y() );
        glEnd();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if 0
        if (_state == eTimelineStateSelectingZoomRange) {
            glColor4f(1, 1, 1, 0.3);
            glBegin(GL_POLYGON);
            glVertex2f( toTimeLine(_mousePressX), btmLeft.y() );
            glVertex2f( toTimeLine(_mousePressX), topRight.y() );
            glVertex2f( toTimeLine(_mouseMoveX), topRight.y() );
            glVertex2f( toTimeLine(_mouseMoveX), btmLeft.y() );
            glEnd();
        }

#endif

        QFontMetrics fm(*_textFont, 0);
        double fmHeight = fm.height() / _screenPixelRatio;
        //        double lineYPosWidget = height() - 1 - fmHeight - 14. / 2.;
        double lineYPosWidget = height() - 1 - fmHeight  - TO_DPIY(TICK_HEIGHT) / 2.;
        double lineYpos = toTimeLineCoordinates(0, lineYPosWidget).y();
        double cachedLineYPos = toTimeLineCoordinates(0, lineYPosWidget + 1).y();

        /*draw the horizontal axis*/
        double txtR, txtG, txtB;
        txtR = 0.78;
        txtG = 0.78;
        txtB = 0.78;
        double kfR, kfG, kfB;
        kfR = 0.08;
        kfG = 0.38;
        kfB = 0.97;
        double userkfR, userkfG, userkfB;
        userkfR = 0.7;
        userkfG = 0.78;
        userkfB = 0.39;
        double cursorR, cursorG, cursorB;
        cursorR = 0.95;
        cursorG = 0.54;
        cursorB = 0.0;

        double boundsR, boundsG, boundsB;
        boundsR = 0.81;
        boundsG = 0.27;
        boundsB = 0.02;

        double cachedR, cachedG, cachedB;
        cachedR = 0.56;
        cachedG = 0.79;
        cachedB = 0.4;

        double dcR, dcG, dcB;
        dcR = 0.27;
        dcG = 0.38;
        dcB = 0.25;

        glColor4f(txtR / 2., txtG / 2., txtB / 2., 1.);
        glLineWidth(1.5 * _screenPixelRatio);
        glBegin(GL_LINES);
        glVertex2f(btmLeft.x(), lineYpos);
        glVertex2f(topRight.x(), lineYpos);
        glEnd();

        double tickBottom = toTimeLineCoordinates( 0, height() - 1 - fmHeight ).y();
        double tickTop = toTimeLineCoordinates( 0, height() - 1 - fmHeight  - TO_DPIY(TICK_HEIGHT) ).y();
        const double smallestTickSizePixel = 30.; // tick size (in pixels) for alpha = 0.
        const double largestTickSizePixel = 200.; // tick size (in pixels) for alpha = 1.
        const double rangePixel =  width();
        const double range_min = btmLeft.x();
        const double range_max =  topRight.x();
        const double range = range_max - range_min;
        const double fps = 30;
        double smallTickSize;
        bool half_tick;

        ticks_size(range_min, range_max, rangePixel, smallestTickSizePixel, &smallTickSize, &half_tick);
        if (smallTickSize < 1) {
            smallTickSize = 1;
            half_tick = false;
        }
        //        std::cout << "rangePixel:" << rangePixel
        //                  << " range_min:" << range_min
        //                  << " range_max:" << range_max
        //                  << " range:" << range
        //                  << " smallTickSize:" << smallTickSize
        //                  << " half_tick:" << half_tick
        //                  << std::endl;

        int m1, m2;
        const int ticks_max = 1000;
        double offset;
        ticks_bounds(range_min, range_max, smallTickSize, half_tick, ticks_max, &offset, &m1, &m2);
        //        std::cout << "range_min:" << range_min
        //                  << " range_max:" << range_max
        //                  << " smallTickSize:" << smallTickSize
        //                  << " half_tick:" << half_tick
        //                  << " ticks_max:" << ticks_max
        //                  << " offset:" << offset
        //                  << " m1:" << m1 << " m2:" << m2
        //                  << std::endl;
        std::vector<int> ticks;
        ticks_fill(half_tick, ticks_max, m1, m2, &ticks);
        const double smallestTickSize = range * smallestTickSizePixel / rangePixel;
        const double largestTickSize = range * largestTickSizePixel / rangePixel;

        const double minTickSizeTextPixel = ( _isTimeFormatFrames ? fm.horizontalAdvance( QLatin1String("00000") ) : fm.horizontalAdvance( QLatin1String("00:00:00:00") ) ) / _screenPixelRatio;

        const double minTickSizeText = range * minTickSizeTextPixel / rangePixel;

        //目盛りの縦線
        for (int i = m1; i <= m2; ++i) {
            double value = i * smallTickSize + offset;
            //            std::cout << "i:" << i << " m1:" << m1 << " m2:" << m2 << " value:" << value << std::endl;
            const double tickSize = ticks[i - m1] * smallTickSize;
            const double alpha = ticks_alpha(smallestTickSize, largestTickSize, tickSize);

            // because smallTickSize is at least 1, isFloating can never be true
            bool isFloating = std::abs(std::floor(0.5 + value) - value) != 0.;
            assert(!isFloating);
            if (isFloating) {
                continue;
            }
            glColor4f(txtR, txtG, txtB, alpha);
            glLineWidth(1.5 * _screenPixelRatio);
            glBegin(GL_LINES);
            glVertex2f(value, tickBottom);
            glVertex2f(value, tickTop);
            glEnd();

            if (tickSize > minTickSizeText) {
                const int tickSizePixel = rangePixel * tickSize / range;
                const QString s = _isTimeFormatFrames ? QString::number(value) : timecodeString(value, fps);

                const int sSizePixel =  fm.horizontalAdvance(s) / _screenPixelRatio;

                if (tickSizePixel > sSizePixel) {
                    const int sSizeFullPixel = sSizePixel + minTickSizeTextPixel;
                    double alphaText = 1.0; //alpha;
                    if (tickSizePixel < sSizeFullPixel) {
                        // when the text size is between sSizePixel and sSizeFullPixel,
                        // draw it with a lower alpha
                        alphaText *= (tickSizePixel - sSizePixel) / (double)minTickSizeTextPixel;
                    }
                    //alphaText = std::min(alphaText, alpha); // don't draw more opaque than ticks
                    QColor c;
                    c.setRgbF( Image::clamp<qreal>(txtR, 0., 1.),
                               Image::clamp<qreal>(txtG, 0., 1.),
                               Image::clamp<qreal>(txtB, 0., 1.) );
                    c.setRgbF(txtR,txtG,txtB);
                    c.setAlpha(255 * alphaText);

                    renderText(value, btmLeft.y(), s, c, *_textFont, Qt::AlignHCenter);
                }
            }
        }



        int cursorWidth = TO_DPIX(CURSOR_WIDTH);
        int cursorHeight = TO_DPIY(CURSOR_HEIGHT);

        QPointF cursorBtm(currentTime, lineYpos);
        QPointF cursorBtmWidgetCoord = toWidgetCoordinates( cursorBtm.x(), cursorBtm.y() );
        QPointF cursorTopLeft = toTimeLineCoordinates(cursorBtmWidgetCoord.x() - cursorWidth / 2.,
                                                      cursorBtmWidgetCoord.y() - cursorHeight);
        QPointF cursorTopRight = toTimeLineCoordinates(cursorBtmWidgetCoord.x() + cursorWidth / 2.,
                                                       cursorBtmWidgetCoord.y() - cursorHeight);
        QPointF leftBoundBtm(leftBound, lineYpos);
        QPointF leftBoundWidgetCoord = toWidgetCoordinates( leftBoundBtm.x(), leftBoundBtm.y() );
        QPointF leftBoundBtmRight = toTimeLineCoordinates( leftBoundWidgetCoord.x() + cursorWidth / 2.,
                                                           leftBoundWidgetCoord.y() );
        QPointF leftBoundTop = toTimeLineCoordinates(leftBoundWidgetCoord.x(),
                                                     leftBoundWidgetCoord.y() - cursorHeight);
        QPointF rightBoundBtm(rightBound, lineYpos);
        QPointF rightBoundWidgetCoord = toWidgetCoordinates( rightBoundBtm.x(), rightBoundBtm.y() );
        QPointF rightBoundBtmLeft = toTimeLineCoordinates( rightBoundWidgetCoord.x() - cursorWidth / 2.,
                                                           rightBoundWidgetCoord.y() );
        QPointF rightBoundTop = toTimeLineCoordinates(rightBoundWidgetCoord.x(),
                                                      rightBoundWidgetCoord.y() - cursorHeight);

        /// pair<time, isUserKey>
        TimeLineKeysSet keyframes;
#if 0

        {
            std::list<SequenceTime> userKeys;
            _gui->getApp()->getUserKeyframes(&userKeys);
            for (std::list<SequenceTime>::iterator it = userKeys.begin(); it != userKeys.end(); ++it) {
                TimeLineKey k(*it, true);
                keyframes.insert(k);
            }

            ///THere may be duplicates in this list
            std::list<SequenceTime> keyframesList;
            _gui->getApp()->getKeyframes(&keyframesList);
            for (std::list<SequenceTime>::iterator it = keyframesList.begin(); it != keyframesList.end(); ++it) {
                TimeLineKey k(*it, false);
                keyframes.insert(k);
            }
        }
#endif

        //draw an alpha cursor if the mouse is hovering the timeline
        glEnable(GL_POLYGON_SMOOTH);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);

        if (_alphaCursor) {
            int currentPosBtmWidgetCoordX = _lastMouseEventWidgetCoord.x();
            int currentPosBtmWidgetCoordY = toWidgetCoordinates(0, lineYpos).y();
            QPointF currentPosBtm = toTimeLineCoordinates(currentPosBtmWidgetCoordX, currentPosBtmWidgetCoordY);
            int hoveredTime = std::floor(currentPosBtm.x() + 0.5);
            QPointF currentBtm(hoveredTime, lineYpos);
            QPointF currentBtmWidgetCoord = toWidgetCoordinates( currentBtm.x(), currentBtm.y() );
            QPointF currentTopLeft = toTimeLineCoordinates(currentBtmWidgetCoord.x() - cursorWidth / 2.,
                                                           currentBtmWidgetCoord.y() - cursorHeight);
            QPointF currentTopRight = toTimeLineCoordinates(currentBtmWidgetCoord.x() + cursorWidth / 2.,
                                                            currentBtmWidgetCoord.y() - cursorHeight);

            QString currentNumber( _isTimeFormatFrames ? QString::number(hoveredTime) : timecodeString(hoveredTime, fps) );
            TimeLineKeysSet::iterator foundHoveredAsKeyframe = keyframes.find( TimeLineKey(hoveredTime) );
            QColor currentColor;
            if ( foundHoveredAsKeyframe != keyframes.end() ) {
                if (foundHoveredAsKeyframe->isUserKey) {
                    glColor4f(userkfR, userkfG, userkfB, 0.4);
                    currentColor.setRgbF( Image::clamp<qreal>(userkfR, 0., 1.),
                                          Image::clamp<qreal>(userkfG, 0., 1.),
                                          Image::clamp<qreal>(userkfB, 0., 1.) );
                } else {
                    glColor4f(kfR, kfG, kfB, 0.4);
                    currentColor.setRgbF( Image::clamp<qreal>(kfR, 0., 1.),
                                          Image::clamp<qreal>(kfG, 0., 1.),
                                          Image::clamp<qreal>(kfB, 0., 1.) );
                }
            } else {
                glColor4f(cursorR, cursorG, cursorB, 0.4);
                currentColor.setRgbF( Image::clamp<qreal>(cursorR, 0., 1.),
                                      Image::clamp<qreal>(cursorG, 0., 1.),
                                      Image::clamp<qreal>(cursorB, 0., 1.) );
            }
            currentColor.setAlpha(100);

            glBegin(GL_POLYGON);
            glVertex2f( currentBtm.x(), currentBtm.y() );
            glVertex2f( currentTopLeft.x(), currentTopLeft.y() );
            glVertex2f( currentTopRight.x(), currentTopRight.y() );
            glEnd();


            renderText(currentBtm.x(), currentTopRight.y(), currentNumber, currentColor, *_textFont, Qt::AlignHCenter);
        }

        //draw the bounds and the current time cursor
        TimeLineKeysSet::iterator isCurrentTimeAKeyframe = keyframes.find( TimeLineKey(currentTime) );
        QColor actualCursorColor;
        if ( isCurrentTimeAKeyframe != keyframes.end() ) {
            if (isCurrentTimeAKeyframe->isUserKey) {
                glColor4f(userkfR, userkfG, userkfB, 1.);
                actualCursorColor.setRgbF( Image::clamp<qreal>(userkfR, 0., 1.),
                                           Image::clamp<qreal>(userkfG, 0., 1.),
                                           Image::clamp<qreal>(userkfB, 0., 1.) );
            } else {
                glColor4f(kfR, kfG, kfB, 1.);
                actualCursorColor.setRgbF( Image::clamp<qreal>(kfR, 0., 1.),
                                           Image::clamp<qreal>(kfG, 0., 1.),
                                           Image::clamp<qreal>(kfB, 0., 1.) );
            }
        } else {
            glColor4f(cursorR, cursorG, cursorB, 1.);
            actualCursorColor.setRgbF( Image::clamp<qreal>(cursorR, 0., 1.),
                                       Image::clamp<qreal>(cursorG, 0., 1.),
                                       Image::clamp<qreal>(cursorB, 0., 1.) );
        }

        QString currentFrameStr = _isTimeFormatFrames ? QString::number(currentTime) : timecodeString(currentTime, fps);
        double cursorTextXposWidget = cursorBtmWidgetCoord.x();
        double cursorTextPos = toTimeLine(cursorTextXposWidget);
        renderText(cursorTextPos, cursorTopLeft.y(), currentFrameStr, actualCursorColor, *_textFont, Qt::AlignHCenter);
        glBegin(GL_POLYGON);
        glVertex2f( cursorBtm.x(), cursorBtm.y() );
        glVertex2f( cursorTopLeft.x(), cursorTopLeft.y() );
        glVertex2f( cursorTopRight.x(), cursorTopRight.y() );
        glEnd();

        QColor boundsColor;
        boundsColor.setRgbF( Image::clamp<qreal>(boundsR, 0., 1.),
                             Image::clamp<qreal>(boundsG, 0., 1.),
                             Image::clamp<qreal>(boundsB, 0., 1.) );


        {
            if ( ( leftBoundBtm.x() >= btmLeft.x() ) && ( leftBoundBtmRight.x() <= topRight.x() ) ) {
                if (leftBound != currentTime) {
                    QString leftBoundStr( _isTimeFormatFrames ? QString::number(leftBound) : timecodeString(leftBound, fps) );
                    double leftBoundTextXposWidget = toWidgetCoordinates( ( leftBoundBtm.x() + leftBoundBtmRight.x() ) / 2., 0 ).x();
                    double leftBoundTextPos = toTimeLine(leftBoundTextXposWidget);
                    renderText(leftBoundTextPos, leftBoundTop.y(),
                               leftBoundStr, boundsColor, *_textFont, Qt::AlignHCenter);
                }
                glColor4f(boundsR, boundsG, boundsB, 1.);
                glBegin(GL_POLYGON);
                glVertex2f( leftBoundBtm.x(), leftBoundBtm.y() );
                glVertex2f( leftBoundBtmRight.x(), leftBoundBtmRight.y() );
                glVertex2f( leftBoundTop.x(), leftBoundTop.y() );
                glEnd();

            }

            if ( ( rightBoundBtmLeft.x() >= btmLeft.x() ) && ( rightBoundBtm.x() <= topRight.x() ) ) {
                if ( (rightBound != currentTime) && (rightBound != leftBound) ) {
                    QString rightBoundStr( _isTimeFormatFrames ? QString::number(rightBound) : timecodeString(rightBound, fps) );
                    double rightBoundTextXposWidget = toWidgetCoordinates( ( rightBoundBtm.x() + rightBoundBtmLeft.x() ) / 2., 0 ).x();
                    double rightBoundTextPos = toTimeLine(rightBoundTextXposWidget);
                    renderText(rightBoundTextPos, rightBoundTop.y(),
                               rightBoundStr, boundsColor, *_textFont, Qt::AlignHCenter);
                }
                glColor4f(boundsR, boundsG, boundsB, 1.);

                glBegin(GL_POLYGON);
                glVertex2f( rightBoundBtm.x(), rightBoundBtm.y() );
                glVertex2f( rightBoundBtmLeft.x(), rightBoundBtmLeft.y() );
                glVertex2f( rightBoundTop.x(), rightBoundTop.y() );
                glEnd();

            }
        }


#if 0
        //draw cached frames
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

        glLineWidth(2 * _screenPixelRatio);
        glBegin(GL_LINES);
        for (CachedFrames::const_iterator i = _cachedFrames.begin(); i != _cachedFrames.end(); ++i) {
            if ( ( i->time >= btmLeft.x() ) && ( i->time <= topRight.x() ) ) {
                if (i->mode == eStorageModeRAM) {
                    glColor4f(cachedR, cachedG, cachedB, 1.);
                } else if (i->mode == eStorageModeDisk) {
                    glColor4f(dcR, dcG, dcB, 1.);
                }
                glVertex2f(i->time, cachedLineYPos);
                glVertex2f(i->time + 1, cachedLineYPos);
            }
        }
        glEnd();

        ///now draw keyframes
        glLineWidth(2 * _screenPixelRatio);
        glBegin(GL_LINES);
        glColor4f(kfR, kfG, kfB, 1.);
        std::list<SequenceTime> remainingUserKeys;
        for (TimeLineKeysSet::const_iterator i = keyframes.begin(); i != keyframes.end(); ++i) {
            if ( ( i->time >= btmLeft.x() ) && ( i->time <= topRight.x() ) ) {
                if (!i->isUserKey) {
                    glVertex2f(i->time, lineYpos);
                    glVertex2f(i->time + 1, lineYpos);
                } else {
                    remainingUserKeys.push_back(i->time);
                }
            }
        }
        glEnd();


        //const double keyHeight = TO_DPIY(USER_KEYFRAMES_HEIGHT);
        //const double keyTop = toTimeLineCoordinates(0, lineYPosWidget - keyHeight).y();
        //const double keyBtm = toTimeLineCoordinates(0, lineYPosWidget + keyHeight).y();
        glColor4f(userkfR, userkfG, userkfB, 1.);
        QColor userKeyColor;
        userKeyColor.setRgbF( Image::clamp<qreal>(userkfR, 0., 1.),
                              Image::clamp<qreal>(userkfG, 0., 1.),
                              Image::clamp<qreal>(userkfB, 0., 1.) );
        for (std::list<SequenceTime>::iterator it = remainingUserKeys.begin(); it != remainingUserKeys.end(); ++it) {
            if (  /* (*it == currentTime) ||*/
                  ( *it == leftBound) ||
                  ( *it == rightBound) ) {
                continue;
            }
            QPointF kfBtm(*it, lineYpos);
            QPointF kfBtmWidgetCoord = toWidgetCoordinates( kfBtm.x(), kfBtm.y() );
            QPointF kfTopLeft = toTimeLineCoordinates(kfBtmWidgetCoord.x() - cursorWidth / 2.,
                                                      kfBtmWidgetCoord.y() - cursorHeight);
            QPointF kfTopRight = toTimeLineCoordinates(kfBtmWidgetCoord.x() + cursorWidth / 2.,
                                                       kfBtmWidgetCoord.y() - cursorHeight);
            QPointF kfBtmLeft = toTimeLineCoordinates(kfBtmWidgetCoord.x() - cursorWidth / 2.,
                                                      kfBtmWidgetCoord.y() + cursorHeight);
            QPointF kfBtmRight = toTimeLineCoordinates(kfBtmWidgetCoord.x() + cursorWidth / 2.,
                                                       kfBtmWidgetCoord.y() + cursorHeight);
            glBegin(GL_POLYGON);
            glVertex2f( kfBtm.x(), cursorBtm.y() );
            glVertex2f( kfTopLeft.x(), kfTopLeft.y() );
            glVertex2f( kfTopRight.x(), kfTopRight.y() );
            glEnd();
            glBegin(GL_POLYGON);
            glVertex2f( kfBtm.x(), cursorBtm.y() );
            glVertex2f( kfBtmLeft.x(), kfBtmLeft.y() );
            glVertex2f( kfBtmRight.x(), kfBtmRight.y() );
            glEnd();

        }
#endif

        glDisable(GL_POLYGON_SMOOTH);
    } // GLProtectAttrib a(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_POLYGON_BIT | GL_LINE_BIT | GL_ENABLE_BIT | GL_HINT_BIT | GL_SCISSOR_BIT | GL_TRANSFORM_BIT);
}//paintGL

void
TimeLineWidget::renderText(double x,
                           double y,
                           const QString & text,
                           const QColor & color,
                           const QFont & font,
                           int flags) const
{
    //    assert( QOpenGLContext::currentContext() == context() );

    //
    if ( text.isEmpty() ) {
        return;
    }

    double w = (double)width();
    double h = (double)height();
    double bottom = _tlZoomCtx.bottom;
    double left = _tlZoomCtx.left;
    double top = bottom +  h / (double)_tlZoomCtx.zoomFactor;
    double right = left +  (w / (double)_tlZoomCtx.zoomFactor);
    if ( (w <= 0) || (h <= 0) || (right <= left) || (top <= bottom) ) {
        return;
    }
    double scalex = (right - left) / (w  * _screenPixelRatio);
    double scaley = (top - bottom) / (h  * _screenPixelRatio);
    _textRenderer.renderText(x, y, scalex, scaley, text, color, font, flags);
    //
}

void
TimeLineWidget::centerOn(SequenceTime left,
                         SequenceTime right,
                         int margin)
{
    double curveWidth = right - left + 2 * margin;
    double w = width();

    _tlZoomCtx.left = left - margin;
    _tlZoomCtx.zoomFactor = w / curveWidth;

    update();
}

void TimeLineWidget::onFrameChanged(SequenceTime,
                                    int reason){
    TimelineChangeReasonEnum r = (TimelineChangeReasonEnum)reason;

    if ( (r == eTimelineChangeReasonUserSeek) && _seekingTimeline ) {
        return;
    }
    update();
}

QPointF
TimeLineWidget::toWidgetCoordinates(double x,
                                    double y) const
{
    double h = (double)height();
    double bottom = _tlZoomCtx.bottom;
    double top =  bottom +  h / _tlZoomCtx.zoomFactor;

    return QPoint( toWidget(x), ( (y - top) / (bottom - top) ) * h );
}

double
TimeLineWidget::toWidget(double t) const
{
    double w = (double)width();
    double left = _tlZoomCtx.left;
    double right = left +  w / _tlZoomCtx.zoomFactor;

    return ( (t - left) / (right - left) ) * w;
}

void
TimeLineWidget::getBounds(SequenceTime* left,
                          SequenceTime* right) const
{
    QMutexLocker k(&_boundariesMutex);
    *left = _leftBoundary;
    *right = _rightBoundary;
}

void TimeLineWidget::setTimeline(const Sww::Core::TimeLinePtr &timeline){

    QObject::connect( timeline.get(), SIGNAL(frameChanged(SequenceTime,int)), this, SLOT(onFrameChanged(SequenceTime,int)), Qt::UniqueConnection );
    _timeline = timeline;
}


Sww::Core::TimeLinePtr TimeLineWidget::getTimeline() const{
    return _timeline;
}

void
TimeLineWidget::setBoundariesInternal(SequenceTime first,
                                      SequenceTime last,
                                      bool emitSignal)
{
    if (first <= last) {
        {
            QMutexLocker k(&_boundariesMutex);
            _leftBoundary = first;
            _rightBoundary = last;
        }
        if (emitSignal) {
            Q_EMIT boundariesChanged(first, last);
        } else {
            update();
        }
        setFrameRangeEdited(true);
    }
}



void
TimeLineWidget::setBoundaries(SequenceTime first,
                              SequenceTime last)
{
    setBoundariesInternal(first, last, false);
}

void
TimeLineWidget::recenterOnBounds()
{
    SequenceTime first, last;
    getBounds(&first, &last);
    centerOn(first, last);
}

bool
TimeLineWidget::isFrameRangeEdited() const
{
    QMutexLocker k(&_frameRangeEditedMutex);
    return _isFrameRangeEdited;
}

void
TimeLineWidget::setFrameRangeEdited(bool edited)
{
    QMutexLocker k(&_frameRangeEditedMutex);
    _isFrameRangeEdited = edited;
}
