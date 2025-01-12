#include "KnobFloat.h"
#include "ui_KnobFloat.h"

#include <Core/Node.h>
#include "Knob/CustomSpinBox.h"
#include "Core/PlugType.h"
#include "Knob/CommandStack.h"
#include "Core/JsonSerializer.h"

KnobFloat::KnobFloat(Sww::Core::PlugBase *plug, QWidget *parent):KnobObject(plug,parent),
    ui(new Ui::KnobFloat)
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
}

KnobFloat::~KnobFloat()
{
    delete ui;
}

void KnobFloat::updateKnob()
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

    if(_plug->prevCount() || _plug->isOutput()) _spinBox->setEnabled(false);
    else  _spinBox->setEnabled(true);

    _spinBox->setValue(value);
}

bool KnobFloat::initialize()
{
    if(_plug->prevCount())
    {
        _spinBox->setEnabled(false);
    }
    //ui->labelPlugName->setText(_plug->getName().c_str());

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
    _spinBox->calcPrecision(value);
    //_spinBox->setMinimum(_plug->getMin());
    _spinBox->setMinimum(-1E15); //_plug->getMin()だとだめだった
    _spinBox->setMaximum(_plug->getMax());

    //SWW_DEBUG_VALUE(_spinBox->minimum());

    return true;
}

KnobObject* KnobFloat::creator(Sww::Core::PlugBase *plug ,QWidget *parent)
{
    return new KnobFloat(plug,parent);
}

void KnobFloat::spinBox_editingFinished()
{
    float f = _spinBox->value();
    SWW_VAL_VOID(_plug,value);
    if(value == f){
        return;
    }

    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    if(!_plug->setInputValue(f)){
       //do nothing.
    }

    cmd->push();
}
