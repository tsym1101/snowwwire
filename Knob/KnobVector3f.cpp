#include "KnobVector3f.h"
#include "ui_KnobVector3f.h"

#include <Core/Node.h>

#include "Knob/CustomSpinBox.h"
#include "Core/PlugType.h"

#include <ImathVec.h>
#include "Knob/CustomSpinBox.h"

KnobVector3f::KnobVector3f(Sww::Core::PlugBase *plug, QWidget *parent):KnobObject(plug,parent),
    ui(new Ui::KnobVector3f)
{
    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::V3F,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::V3fPlug*>(plug);
    //SWW_DEBUG_VALUE(_plug->getSwwTypeDescString());
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");

    for(int i = 0;i<3;i++)
    {
        _spinBoxes[i] = new CustomSpinBox(this);
        ui->horizontalLayout->insertWidget(i,_spinBoxes[i]);
    }

    if(plug->isOutput()){//outputは編集できない
        for(int i = 0;i < 3;i++){
            _spinBoxes[i]->setEnabled(false);
        }
    }

    connect(_spinBoxes[0],SIGNAL(editingFinished()),this,SLOT(x_editingFinished()));
    connect(_spinBoxes[1],SIGNAL(editingFinished()),this,SLOT(y_editingFinished()));
    connect(_spinBoxes[2],SIGNAL(editingFinished()),this,SLOT(z_editingFinished()));
}

KnobVector3f::~KnobVector3f()
{
    delete ui;
}

bool KnobVector3f::initialize()
{
    if(_plug->prevCount())
    {
        _spinBoxes[0]->setEnabled(false);
        _spinBoxes[1]->setEnabled(false);
        _spinBoxes[2]->setEnabled(false);
    }

    Imath::V3f value;
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        const Imath::V3f& valueTmp = _plug->getConstInputValue(&ok);
        if(!ok){
            SWW_ERR("getInputValue() failed." << _plug->getDebugName());
            return false;
        }
        value = valueTmp;
    }

    _spinBoxes[0]->setValue(value.x);
    _spinBoxes[1]->setValue(value.y);
    _spinBoxes[2]->setValue(value.z);

    return true;
}


KnobObject* KnobVector3f::creator(Sww::Core::PlugBase *plug ,QWidget *parent)
{
    return new KnobVector3f(plug,parent);
}

void KnobVector3f::setFromSpinbox(int target)
{
    float f = _spinBoxes[target]->value();

    Q_ASSERT(!isOutput());
    SWW_VAL_VOID(_plug,value);
    if(f == value[target])return;

    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    ((float*)&value)[target] = f;

    if(!_plug->setInputValue(value))
    {
        SWW_ERR("setFromSpinbox failed. " << _plug->getDebugName());
    }
    cmd->push();
}

void KnobVector3f::x_editingFinished()
{
    setFromSpinbox(0);
}
void KnobVector3f::y_editingFinished()
{
    setFromSpinbox(1);
}
void KnobVector3f::z_editingFinished()
{
    setFromSpinbox(2);
}

void KnobVector3f::updateKnob()
{
    Imath::V3f value;
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        const Imath::V3f& valueTmp = _plug->getConstInputValue(&ok);
        if(!ok){
            SWW_ERR("postChangeState failed. " << _plug->getDebugName());
            return;
        }
        value = valueTmp;
    }

    _spinBoxes[0]->setValue(value.x);
    _spinBoxes[1]->setValue(value.y);
    _spinBoxes[2]->setValue(value.z);


    if(_plug->prevCount() || _plug->isOutput())
    {
        for(int i = 0;i < 3;i++){
            _spinBoxes[i]->setEnabled(false);
        }
    }else  {
        for(int i = 0;i < 3;i++){
            _spinBoxes[i]->setEnabled(true);
        }
    }

}


