#ifndef TIMESLIDER_H
#define TIMESLIDER_H

#include <GL/glew.h>

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif
#include <QTimer>

//#include <boost/scoped_ptr.hpp>
//#include <boost/system/error_code.hpp>

namespace Ui {
class TimeSlider;
}

namespace SwwGui{
class NodeGraphScene;
}

namespace Sww::Core{
class RootNode;
}

class TimeSlider : public QWidget
{
    Q_OBJECT
public:
    friend class CustomTimer;

    TimeSlider(Sww::Core::RootNode* rootNode,QWidget *parent = 0);
    ~TimeSlider();

private Q_SLOTS:
    void on_spinBoxEndFrame_editingFinished();
    void on_spinBoxStartFrame_editingFinished();
    void on_spinBoxCurrentFrame_editingFinished();
//    void on_horizontalSlider_sliderMoved(int value);
    void on_pushButtonPlay_clicked();
    void animate();
    void setToGui();
    void on_doubleSpinBoxFps_editingFinished();

    void on_pushButtonForward_clicked();

    void on_pushButtonSkipForward_clicked();

    void on_pushButtonBackward_clicked();

    void on_pushButtonSkipBackward_clicked();


private:
    Ui::TimeSlider *ui;
    //SwwGui::NodeGraphScene* _nodeGraphScene;
    Sww::Core::RootNode* _rootNode;

    bool _isRunning;
    QTimer _animationTimer;

    double _fps; //frames per second
    double _mspf; //milli seconds per frame

    //    boost::scoped_ptr<boost::asio::io_service>  _io_service;
    //    boost::scoped_ptr<boost::asio::steady_timer>  _timer;

};

#endif // TIMESLIDER_H
