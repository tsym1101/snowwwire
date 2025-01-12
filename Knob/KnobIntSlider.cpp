#include "KnobIntSlider.h"
#include "ui_KnobIntSlider.h"

#include <Core/Node.h>
#include "Core/PlugType.h"

KnobIntSlider::KnobIntSlider(Sww::Core::PlugBase *plug, QWidget *parent):KnobObject(plug,parent),
    _setValueAlreadyCalled(false),
    ui(new Ui::KnobIntSlider)
{
    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::INT,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::IntPlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");


    if(plug->isOutput())ui->spinBox->setEnabled(false);//outputは編集できない
    connect(ui->spinBox,SIGNAL(editingFinished()),this,SLOT(spinBox_editingFinished()));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(slider_valueChanged(int)));

}

KnobIntSlider::~KnobIntSlider()
{
    delete ui;
}

bool KnobIntSlider::initialize()
{
    if(_plug->prevCount())
    {
        ui->spinBox->setEnabled(false);
    }
    //ui->labelPlugName->setText(_plug->getName().c_str());

    int value;
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        value = _plug->getConstInputValue(&ok);
        if(!ok){
            SWW_ERR("getInputValue() failed." << _plug->getDebugName());
            return false;
        }
    }

    ui->spinBox->setValue(value);
    ui->spinBox->setMinimum(_plug->getMin());
    ui->spinBox->setMaximum(_plug->getMax());
    _setValueAlreadyCalled = true;
    ui->horizontalSlider->setValue(value);
    ui->horizontalSlider->setMinimum(_plug->getMin());
    ui->horizontalSlider->setMaximum(_plug->getMax());

    return true;
}

KnobObject* KnobIntSlider::creator(Sww::Core::PlugBase *plug ,QWidget *parent)
{
    return new KnobIntSlider(plug,parent);
}

void KnobIntSlider::slider_valueChanged(int value) {
    if(!_setValueAlreadyCalled){
        if(!_plug->setInputValue(value)){
            SWW_ERR("setInputValue failed." << _plug->getDebugName());
            return;
        }
        ui->spinBox->setValue(value);
        //SWW_DEBUG_VALUE(value);
    }
    else{
        _setValueAlreadyCalled = false;
    }
}

void KnobIntSlider::updateKnob()
{
    Q_ASSERT(!_plug->dataIsNull());

    int value;
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        value = _plug->getConstInputValue(&ok);
        if(!ok){
            SWW_ERR("postChangeState failed. " << _plug->getDebugName());
            return;
        }
    }

    if(_plug->prevCount() || _plug->isOutput()){
        ui->spinBox->setEnabled(false);
        ui->horizontalSlider->setEnabled(false);
    }
    else{
        ui->spinBox->setEnabled(true);
        ui->horizontalSlider->setEnabled(true);
    }

    ui->spinBox->setValue(value);
    _setValueAlreadyCalled = true;
    ui->horizontalSlider->setValue(value);
}

void KnobIntSlider::spinBox_editingFinished()
{
    int value = ui->spinBox->value();    
    SWW_VAL_VOID(_plug,currentValue);
    if(value == currentValue){
        return;
    }

    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    if(!_plug->setInputValue(value))return;
    _setValueAlreadyCalled = true;
    ui->horizontalSlider->setValue(value);

    cmd->push();

}

void KnobIntSlider::on_horizontalSlider_sliderPressed()
{
    _cmdTmp = new SwwKnob::UndoCommand(_plug);
    _cmdTmp->storeUndoValue();
}

void KnobIntSlider::on_horizontalSlider_sliderReleased()
{
    _cmdTmp->push();
}
