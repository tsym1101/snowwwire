#include "KnobVector4f.h"
#include "ui_KnobVector4f.h"

#include <Core/Node.h>

#include "Knob/CustomSpinBox.h"
#include "Core/PlugType.h"

#include <ImathVec.h>
#include "Knob/CustomSpinBox.h"

KnobVector4f::KnobVector4f(Sww::Core::PlugBase *plug, QWidget *parent):KnobObject(plug,parent),
    ui(new Ui::KnobVector4f)
{
    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::V4F,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::V4fPlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");

    for(int i = 0;i<4;i++)
    {
        _spinBoxes[i] = new CustomSpinBox(this);
        ui->horizontalLayout->insertWidget(i,_spinBoxes[i]);
    }

    if(plug->isOutput()){//outputは編集できない
        for(int i = 0;i < 4;i++){
            _spinBoxes[i]->setEnabled(false);
        }
    }

    connect(_spinBoxes[0],SIGNAL(editingFinished()),this,SLOT(x_editingFinished()));
    connect(_spinBoxes[1],SIGNAL(editingFinished()),this,SLOT(y_editingFinished()));
    connect(_spinBoxes[2],SIGNAL(editingFinished()),this,SLOT(z_editingFinished()));
    connect(_spinBoxes[3],SIGNAL(editingFinished()),this,SLOT(w_editingFinished()));
}

KnobVector4f::~KnobVector4f()
{
    delete ui;
}

bool KnobVector4f::initialize()
{
    if(_plug->prevCount())
    {
        _spinBoxes[0]->setEnabled(false);
        _spinBoxes[1]->setEnabled(false);
        _spinBoxes[2]->setEnabled(false);
        _spinBoxes[3]->setEnabled(false);
    }

    Imath::V4f value;
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        const Imath::V4f& valueTmp = _plug->getConstInputValue(&ok);
        if(!ok){
            SWW_ERR("getInputValue() failed." << _plug->getDebugName());
            return false;
        }
        value = valueTmp;
    }

    _spinBoxes[0]->setValue(value.x);
    _spinBoxes[1]->setValue(value.y);
    _spinBoxes[2]->setValue(value.z);
    _spinBoxes[3]->setValue(value.w);

    return true;
}


KnobObject* KnobVector4f::creator(Sww::Core::PlugBase *plug ,QWidget *parent)
{
    return new KnobVector4f(plug,parent);
}

void KnobVector4f::setFromSpinbox(int target)
{
    float f = _spinBoxes[target]->value();

    Q_ASSERT(!isOutput());
    SWW_VAL_VOID(_plug,value);
    if(f == value[target])return;

    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    ((float*)&value)[target] = f;
    if(_plug->setInputValue(value) == false)
    {
        SWW_ERR("setFromSpinbox failed. " << _plug->getDebugName());
    }

    cmd->push();

}

void KnobVector4f::x_editingFinished()
{
    setFromSpinbox(0);
}
void KnobVector4f::y_editingFinished()
{
    setFromSpinbox(1);
}
void KnobVector4f::z_editingFinished()
{
    setFromSpinbox(2);
}
void KnobVector4f::w_editingFinished()
{
    setFromSpinbox(3);
}

void KnobVector4f::updateKnob()
{
    Imath::V4f value;
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        const Imath::V4f& valueTmp = _plug->getConstInputValue(&ok);
        if(!ok){
            SWW_ERR("postChangeState failed. " << _plug->getDebugName());
            return;
        }
        value = valueTmp;
    }

    _spinBoxes[0]->setValue(value.x);
    _spinBoxes[1]->setValue(value.y);
    _spinBoxes[2]->setValue(value.z);
    _spinBoxes[3]->setValue(value.w);

    if(_plug->prevCount() || _plug->isOutput())
    {
        for(int i = 0;i < 4;i++){
            _spinBoxes[i]->setEnabled(false);
        }
    }else  {
        for(int i = 0;i < 4;i++){
            _spinBoxes[i]->setEnabled(true);
        }
    }

}
