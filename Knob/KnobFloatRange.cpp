#include "KnobFloatRange.h"
#include "ui_KnobFloatRange.h"

#include <Core/Node.h>
#include "Knob/FloatRangeSlider.h"
#include "Core/PlugType.h"

#include "Knob/CustomSpinBox.h"

KnobFloatRange::KnobFloatRange(Sww::Core::PlugBase *plug, QWidget *parent):KnobObject(plug,parent),
    ui(new Ui::KnobFloatRange)
{
    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::V2F,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::V2fPlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");

    _slider = new SwwKnob::FloatRangeSlider();
    ui->horizontalLayout->addWidget(_slider);

    if(plug->isOutput())_slider->setEnabled(false);//outputは編集できない
    connect(_slider,SIGNAL(valuesChanged(float,float)),this,SLOT(slider_valueChanged(float,float)));
}

KnobFloatRange::~KnobFloatRange()
{
    delete ui;
}

void KnobFloatRange::updateKnob()
{
    Q_ASSERT(!_plug->dataIsNull());

    Imath::V2f value;
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

    if(_plug->prevCount() || _plug->isOutput()) _slider->setEnabled(false);
    else  _slider->setEnabled(true);

    _slider->setValues(value.x,value.y);
}

bool KnobFloatRange::initialize()
{
    if(_plug->prevCount())
    {
        _slider->setEnabled(false);
    }

    Imath::V2f value;
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
    _slider->setValues(value.x,value.y);
    _slider->setMinValue(0.f);
    _slider->setMaxValue(1.f);

    return true;
}

KnobObject* KnobFloatRange::creator(Sww::Core::PlugBase *plug ,QWidget *parent)
{
    return new KnobFloatRange(plug,parent);
}

void KnobFloatRange::slider_valueChanged(float v0, float v1)
{
    SWW_VAL_VOID(_plug,currentValue);
    if(currentValue.x == v0 && currentValue.y == v1){
        return;
    }

    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

   if(!_plug->setInputValue(Imath::V2f(v0,v1))){

   }

    cmd->push();
}
