#include "KnobBaseWidget.h"
#include "ui_KnobBaseWidget.h"

#include "KnobObject.h"

KnobBaseWidget::KnobBaseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KnobBaseWidget)
{
    ui->setupUi(this);
}

KnobBaseWidget::~KnobBaseWidget()
{
    delete ui;
}

void KnobBaseWidget::setKnob(QWidget *knob){
    ui->horizontalLayoutKnob->addWidget(knob);
    KnobObject* knobPtr = dynamic_cast<KnobObject*>(knob);
    Q_ASSERT(knobPtr);
    _knob = knobPtr;

    if(knobPtr->needNameTag()){
        ui->labelPlugName->setText(knobPtr->getName().c_str());
    }
    else{
        ui->labelPlugName->setHidden(true);
    }
}

bool KnobBaseWidget::hasKnob(const std::string& knobName)const{

   return  _knob->getName() == knobName;
}
