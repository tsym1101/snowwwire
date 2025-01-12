#include "KnobVector3i.h"
#include "ui_KnobVector3i.h"

#include <Core/Node.h>

//#include "Knob/CustomSpinBox.h"
#include "Core/PlugType.h"

#include <ImathVec.h>

KnobVector3i::KnobVector3i(Sww::Core::PlugBase *plug, QWidget *parent):KnobObject(plug,parent),
    ui(new Ui::KnobVector3i)
{
    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::V3I,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::V3iPlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");

    for(int i = 0;i<3;i++)
    {
        _spinBoxes[i] = new QSpinBox(this);
        ui->horizontalLayout->insertWidget(i,_spinBoxes[i]);
        int max = std::numeric_limits<int>::max();
        int min = std::numeric_limits<int>::min();

        _spinBoxes[i]->setMaximum(max);
        _spinBoxes[i]->setMinimum(min);
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

KnobVector3i::~KnobVector3i()
{
    delete ui;
}

bool KnobVector3i::initialize()
{
    if(_plug->prevCount())
    {
        _spinBoxes[0]->setEnabled(false);
        _spinBoxes[1]->setEnabled(false);
        _spinBoxes[2]->setEnabled(false);
    }

    //ui->labelPlugName->setText(_plug->getName().c_str());

    Imath::V3i value;
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        const Imath::V3i& valueTmp = _plug->getConstInputValue(&ok);
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


KnobObject* KnobVector3i::creator(Sww::Core::PlugBase *plug ,QWidget *parent)
{
    return new KnobVector3i(plug,parent);
}

void KnobVector3i::setFromSpinbox(int target)
{
    int i = _spinBoxes[target]->value();

    Q_ASSERT(!isOutput());
    SWW_VAL_VOID(_plug,value);
    if(i == value[target])return;

    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    ((int*)&value)[target] = i;

    if(!_plug->setInputValue(value))
    {
        SWW_ERR("setFromSpinbox failed. " << _plug->getDebugName());
    }
    cmd->push();
}

void KnobVector3i::x_editingFinished()
{
    setFromSpinbox(0);
}

void KnobVector3i::y_editingFinished()
{
    setFromSpinbox(1);
}
void KnobVector3i::z_editingFinished()
{
    setFromSpinbox(2);
}

void KnobVector3i::updateKnob()
{
    Imath::V3i value;
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        const Imath::V3i& valueTmp = _plug->getConstInputValue(&ok);
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


