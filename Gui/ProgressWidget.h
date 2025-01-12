#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif

#include "Gui/GuiAPI.h"

namespace Ui {
class ProgressWidget;
}

class SWWGUISHARED_EXPORT ProgressWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressWidget(bool* stopped ,QWidget *parent = 0);
    ~ProgressWidget();

    void setInitialStatus(int start, int end, const QString &plugName);

    //bool* stoppedPtr(){return &_stopped;}
    
public Q_SLOTS:
    void setCurrentStatus(int current , int progress);

protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *);

private:
    Ui::ProgressWidget *ui;

    bool* _stopped;
};

#endif // PROGRESSWIDGET_H
