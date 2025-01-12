#include "KnobFloatSlider.h"
#include "ui_KnobFloatSlider.h"

#include <Core/Node.h>
#include "Knob/CustomSpinBox.h"
#include "Core/PlugType.h"

KnobFloatSlider::KnobFloatSlider(Sww::Core::PlugBase *plug, QWidget *parent):KnobObject(plug,parent),
    _setValueAlreadyCalled(false),_cmdTmp(0),
    ui(new Ui::KnobFloatSlider)
{
    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::FLOAT,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::FloatPlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");

    _spinBox = new CustomSpinBox();
    ui->horizontalLayout->insertWidget(0,_spinBox);
    if(plug->isOutput())_spinBox->setEnabled(false);//outputは編集できない
    connect(_spinBox,SIGNAL(editingFinished()),this,SLOT(spinBox_editingFinished()));
    //_spinBox->setPrecision(3);

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)),this, SLOT(notifyValueChanged(int)));
}

KnobFloatSlider::~KnobFloatSlider()
{
    delete ui;
}

bool KnobFloatSlider::initialize()
{
    if(_plug->prevCount())
    {
        _spinBox->setEnabled(false);
    }

    float value;
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

    _spinBox->setValue(value);
    _spinBox->setMinimum(_plug->getMin());
    _spinBox->setMaximum(_plug->getMax());
    _setValueAlreadyCalled = true;
    ui->horizontalSlider->setValue(value * 1000.0);
    ui->horizontalSlider->setMinimum(_plug->getMin() * 1000.0);
    ui->horizontalSlider->setMaximum(_plug->getMax() * 1000.0);

    return true;
}

KnobObject* KnobFloatSlider::creator(Sww::Core::PlugBase *plug ,QWidget *parent)
{
    return new KnobFloatSlider(plug,parent);
}

void KnobFloatSlider::updateKnob()
{
    Q_ASSERT(!_plug->dataIsNull());

    float value;
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
        _spinBox->setEnabled(false);
        ui->horizontalSlider->setEnabled(false);
    }
    else{
        _spinBox->setEnabled(true);
        ui->horizontalSlider->setEnabled(true);
    }

    _spinBox->setValue(value);
    _setValueAlreadyCalled = true;
    ui->horizontalSlider->setValue(value * 1000.0);
}

void KnobFloatSlider::notifyValueChanged(int value) {
    if(!_setValueAlreadyCalled){
        double doubleValue = double(value) / 1000.0;

        if(!_plug->setInputValue(doubleValue)){
            SWW_ERR("setInputValue failed." << _plug->getDebugName());
            return;
        }
        _spinBox->setValue(doubleValue);

    }
    else{
        _setValueAlreadyCalled = false;
    }
}

void KnobFloatSlider::spinBox_editingFinished()
{
    double value = _spinBox->value();
    SWW_VAL_VOID(_plug,currentValue);
    if(float(value) == currentValue){
        return;
    }

    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    if(!_plug->setInputValue(value))return;
    _setValueAlreadyCalled = true;
    ui->horizontalSlider->setValue(value * 1000.0);

    cmd->push();
}

void KnobFloatSlider::on_horizontalSlider_sliderPressed()
{
    _cmdTmp = new SwwKnob::UndoCommand(_plug);
    _cmdTmp->storeUndoValue();
}

void KnobFloatSlider::on_horizontalSlider_sliderReleased()
{
    _cmdTmp->push();
}
