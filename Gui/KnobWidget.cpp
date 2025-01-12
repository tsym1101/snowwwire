#include "KnobWidget.h"
#include "ui_KnobWidget.h"

#include "Gui/KnobMaker.h"
#include "Core/SwwAssert.h"

//#include "SwwGlobal/SwwGlobal.h"

//#include "Knob/KnobBaseWidget.h"

KnobWidget::KnobWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KnobWidget)
{
    ui->setupUi(this);
}

KnobWidget::~KnobWidget()
{
    clear();
    delete ui;
}

void KnobWidget::addKnob(QWidget *widget)
{
    ui->verticalLayoutChild->addWidget(widget);
    widget->show();
}

void KnobWidget::clear()
{
    QLayoutItem* item;
    while ( ( item =  ui->verticalLayoutChild->takeAt( 0 ) ) != NULL )
    {
        item->widget()->setParent(0);//itemの型はKnobGroupWidget,所有権はNodeItem
        item->widget()->hide();
    }
}
