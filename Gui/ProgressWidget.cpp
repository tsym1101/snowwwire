#include "ProgressWidget.h"
#include "ui_ProgressWidget.h"

#include <QDebug>
#include <QKeyEvent>

ProgressWidget::ProgressWidget(bool* stopped, QWidget *parent) :
    QWidget(parent),_stopped(stopped),
    ui(new Ui::ProgressWidget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Tool
    | Qt::WindowTitleHint
    | Qt::WindowMaximizeButtonHint
    | Qt::CustomizeWindowHint);

    //setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    //setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
}

ProgressWidget::~ProgressWidget()
{
    delete ui;
}

void ProgressWidget::setInitialStatus(int start, int end,const QString &plugName)
{
    ui->labelStartFrame->setText(QString::number(start));
    ui->labelEndFrame->setText(QString::number(end));
    QString nodeLabel = QString(":") + plugName ;
    ui->labelNodeName->setText(nodeLabel);
}

void ProgressWidget::setCurrentStatus(int current, int progress)
{
    ui->labelCurrentFrame->setText(QString::number(current));
    ui->progressBar->setValue(progress);

}

void ProgressWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_Escape:{
       *_stopped = true;
    }
    }//switch

    QWidget::keyPressEvent(event);
}

void ProgressWidget::closeEvent(QCloseEvent *)
{
    *_stopped = true;
}


