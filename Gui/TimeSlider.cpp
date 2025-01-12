#include "TimeSlider.h"
#include "ui_TimeSlider.h"

#include "Core/SwwAssert.h"

#include <QElapsedTimer>
#include "Core/RootNode.h"

#include "TimeLine.h"

//#define CHECK_FRAME_RATE
#ifdef CHECK_FRAME_RATE
QElapsedTimer g_et;
#endif

TimeSlider::TimeSlider(Sww::Core::RootNode *rootNode, QWidget *parent) :
    _rootNode(rootNode),QWidget(parent),_isRunning(false),_fps(30.0),_mspf(1000.0/30.0),
    ui(new Ui::TimeSlider)
{
    ui->setupUi(this);
    QString style = "QAbstractSpinBox{"
                    "min-width: 80px;"
                    "max-width: 80px;"
                    "padding-right: 0px;"
                    "border: 0px"
                    "}"
                    "QSlider{"
                    "min-width: 200px;"
                    "}"
                    "QSlider::groove:horizontal {"
                    "    border: 0px solid #999999;"
                    "    height: 5px; /* the groove expands to the size of the slider by default. by giving it a height, it has a fixed size */"
                    "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
                    "    margin: 0px 0;"
                    "    border-radius: 1px;"
                    "}"
                    "QSlider::handle:horizontal {"
                    "    /*background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);*/"
                    "   background: rgb(180, 180, 180);"
                    "    border: 1px solid #5c5c5c;"
                    "    width: 8px;"
                    "    margin: -10px 0;"
                    "    border-radius: 0px;"
                    "}"
                    "QPushButton {"
                    "border: 0px;"
                    "margin: 0px 0;"
                    "}";

    setStyleSheet(style);

    _animationTimer.setSingleShot(false);
    connect(&_animationTimer, SIGNAL(timeout()), this, SLOT(animate()));
    connect(rootNode,SIGNAL(frameRangeChanged()),this,SLOT(setToGui()));

    //    if(parent){
    //        connect(parent,SIGNAL(closed()),this,SLOT(on_pushButtonPlay_clicked()));
    //    }

    QIcon playIcon = QApplication::style()->standardIcon( QStyle::SP_MediaPlay );
    ui->pushButtonPlay->setIcon(playIcon);

    QIcon forwardIcon = QApplication::style()->standardIcon( QStyle::SP_MediaSeekForward );
    ui->pushButtonForward->setIcon(forwardIcon);

    QIcon backwardIcon = QApplication::style()->standardIcon( QStyle::SP_MediaSeekBackward );
    ui->pushButtonBackward->setIcon(backwardIcon);

    QIcon skipForwardIcon = QApplication::style()->standardIcon( QStyle::SP_MediaSkipForward );
    ui->pushButtonSkipForward->setIcon(skipForwardIcon);

    QIcon skipBackwardIcon = QApplication::style()->standardIcon( QStyle::SP_MediaSkipBackward );
    ui->pushButtonSkipBackward->setIcon(skipBackwardIcon);
    //    _io_service.reset(new  boost::asio::io_service());
    //    _timer.reset(new  boost::asio::steady_timer(*_io_service));

    Sww::Core::TimeLinePtr timeline = std::make_shared<Sww::Core::TimeLine>();
    ui->timelineWidget->setTimeline(timeline);
    ui->timelineWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    ui->timelineWidget->setFrameRangeEdited(false);

    setToGui();

#ifdef CHECK_FRAME_RATE
    g_et.start();
#endif
}

TimeSlider::~TimeSlider()
{
    delete ui;
}

void TimeSlider::on_spinBoxEndFrame_editingFinished()
{
    int value = ui->spinBoxEndFrame->value();
    //_nodeGraphScene->nodeGraph()->endFrame(value);
    //_nodeGraphScene->nodeGraph()->adaptFrameRange(false);

    _rootNode->endFrame(value);
    _rootNode->adaptFrameRange(false);

    setToGui();
}

void TimeSlider::on_spinBoxStartFrame_editingFinished()
{
    int value = ui->spinBoxStartFrame->value();
    //    _nodeGraphScene->nodeGraph()->startFrame(value);
    //    _nodeGraphScene->nodeGraph()->adaptFrameRange(true);

    _rootNode->startFrame(value);
    _rootNode->adaptFrameRange(true);

    setToGui();
}

void TimeSlider::on_spinBoxCurrentFrame_editingFinished()
{
    int value = ui->spinBoxCurrentFrame->value();
    _rootNode->setGlobalFrame(value);
//    ui->horizontalSlider->setValue(value);
}

void TimeSlider::setToGui()
{
    int current = _rootNode->currentFrame();
    int start = _rootNode->startFrame();
    int end = _rootNode->endFrame();

    ui->spinBoxCurrentFrame->setMinimum(start);
//    ui->horizontalSlider->setMinimum(start);
    ui->spinBoxStartFrame->setValue(start);

    ui->spinBoxCurrentFrame->setMaximum(end);
//    ui->horizontalSlider->setMaximum(end);
    ui->spinBoxEndFrame->setValue(end);

//    ui->horizontalSlider->setValue(current); //spinBoxCurrentFrameも変更

    ui->timelineWidget->setBoundaries(start,end);
    ui->timelineWidget->recenterOnBounds();

}

//void TimeSlider::on_horizontalSlider_sliderMoved(int value)
//{
//    _rootNode->setGlobalFrame(value);
//    ui->spinBoxCurrentFrame->setValue(value);
//}

void TimeSlider::on_pushButtonPlay_clicked()
{
    if(!_isRunning)
    {
        _isRunning =true;
        QIcon playIcon = QApplication::style()->standardIcon( QStyle::SP_MediaStop );
        ui->pushButtonPlay->setIcon(playIcon);
        _animationTimer.start(_mspf);
        //        _timer->expires_from_now(std::chrono::milliseconds(100));
        //        _timer->async_wait(boost::bind(&TimeSlider::animate2, this,boost::asio::placeholders::error, _timer.get() ));
        //        _io_service->run();
    }else
    {
        _isRunning = false;
        QIcon playIcon = QApplication::style()->standardIcon( QStyle::SP_MediaPlay );
        ui->pushButtonPlay->setIcon(playIcon);
        _animationTimer.stop();
        // _io_service->stop();
    }
}

void TimeSlider::animate()
{
#ifdef CHECK_FRAME_RATE
    SWW_DEBUG("interval:" << g_et.elapsed());
    SWW_DEBUG_VALUE(_animationTimer.interval());
#endif

    int current,start,end;

    current = _rootNode->currentFrame();
    start = _rootNode->startFrame();
    end = _rootNode->endFrame();

    current++;
    if(current > end){
        current = start;

    }

    _rootNode->setGlobalFrame(current);
//    ui->horizontalSlider->setValue(current);
    ui->spinBoxCurrentFrame->setValue(current);
    ui->timelineWidget->getTimeline()->seekFrame(current, Sww::Core::eTimelineChangeReasonPlaybackSeek);

#ifdef CHECK_FRAME_RATE
    g_et.restart();
#endif
}

void TimeSlider::on_doubleSpinBoxFps_editingFinished()
{
    _fps = ui->doubleSpinBoxFps->value();
    _mspf = 1000.0 / _fps;
    if(_isRunning){
        //        _animationTimer.stop();
        //        _animationTimer.start(_mspf);
        _animationTimer.setInterval(_mspf);
    }
}

void TimeSlider::on_pushButtonForward_clicked()
{
    int end = _rootNode->endFrame();
    int current = _rootNode->currentFrame();
    if(current < end){
        current++;
        _rootNode->setGlobalFrame(current);
//        ui->horizontalSlider->setValue(current);
        ui->spinBoxCurrentFrame->setValue(current);
    }
}

void TimeSlider::on_pushButtonSkipForward_clicked()
{
    int end = _rootNode->endFrame();
    int current = _rootNode->currentFrame();
    if(current < end){
        current = end;
        _rootNode->setGlobalFrame(current);
//        ui->horizontalSlider->setValue(current);
        ui->spinBoxCurrentFrame->setValue(current);
    }
}

void TimeSlider::on_pushButtonBackward_clicked()
{
    int start = _rootNode->startFrame();
    int current = _rootNode->currentFrame();
    if(current > start){
        current--;
        _rootNode->setGlobalFrame(current);
//        ui->horizontalSlider->setValue(current);
        ui->spinBoxCurrentFrame->setValue(current);
    }
}

void TimeSlider::on_pushButtonSkipBackward_clicked()
{
    int start = _rootNode->startFrame();
    int current = _rootNode->currentFrame();
    if(current > start){
        current = start;
        _rootNode->setGlobalFrame(current);
//        ui->horizontalSlider->setValue(current);
        ui->spinBoxCurrentFrame->setValue(current);
    }

}

