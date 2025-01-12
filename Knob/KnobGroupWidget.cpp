#include "KnobGroupWidget.h"
#include "ui_KnobGroupWidget.h"
#include "Knob/KnobBaseWidget.h"
#include "Core/SwwAssert.h"
#include "Knob/KnobObject.h"

KnobGroupWidget::KnobGroupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KnobGroupWidget)
{
    ui->setupUi(this);
}

KnobGroupWidget::~KnobGroupWidget()
{
    delete ui;
}

void KnobGroupWidget::setNodeName(const QString& nodeName){
    ui->labelNodeName->setText(nodeName);
}

void KnobGroupWidget::addKnobBase(KnobBaseWidget *knobBase){
    Q_ASSERT(knobBase);
    //Q_ASSERT(dynamic_cast<KnobBaseWidget*>(knobBase));
    //int numChildren = ui->verticalLayout->children().size();
    ui->verticalLayout->addWidget(knobBase);
    //_children.push_back(knobBase);
}

void KnobGroupWidget::removeWidget(KnobBaseWidget *knobBase){
    ui->verticalLayout->removeWidget(knobBase);
    //_children.removeOne(knobBase);
}

//void KnobGroupWidget::updateChildren(){

//    Q_FOREACH(KnobBaseWidget* w,_children){
//        w->knob()->updateKnob();
//    }

//}
