#include "KnobVector2i.h"
#include "ui_KnobVector2i.h"

#include <Core/Node.h>

//#include "Knob/CustomSpinBox.h"
#include "Core/PlugType.h"

#include <ImathVec.h>

KnobVector2i::KnobVector2i(Sww::Core::PlugBase *plug, QWidget *parent):KnobObject(plug,parent),
    ui(new Ui::KnobVector2i)
{
    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::V2I,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::V2iPlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");

    for(int i = 0;i<2;i++)
    {
        _spinBoxes[i] = new QSpinBox(this);
        ui->horizontalLayout->insertWidget(i,_spinBoxes[i]);
        int max = std::numeric_limits<int>::max();
        int min = std::numeric_limits<int>::min();

        _spinBoxes[i]->setMaximum(max);
        _spinBoxes[i]->setMinimum(min);
    }

    if(plug->isOutput()){//outputは編集できない
        for(int i = 0;i < 2;i++){
            _spinBoxes[i]->setEnabled(false);
        }
    }

    connect(_spinBoxes[0],SIGNAL(editingFinished()),this,SLOT(x_editingFinished()));
    connect(_spinBoxes[1],SIGNAL(editingFinished()),this,SLOT(y_editingFinished()));
    //connect(_spinBoxes[2],SIGNAL(editingFinished()),this,SLOT(z_editingFinished()));
}

KnobVector2i::~KnobVector2i()
{
    delete ui;
}

bool KnobVector2i::initialize()
{
    if(_plug->prevCount())
    {
        _spinBoxes[0]->setEnabled(false);
        _spinBoxes[1]->setEnabled(false);        
    }

    Imath::V2i value;
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        const Imath::V2i& valueTmp = _plug->getConstInputValue(&ok);
        if(!ok){
            SWW_ERR("getInputValue() failed." << _plug->getDebugName());
            return false;
        }
        value = valueTmp;
    }

    _spinBoxes[0]->setValue(value.x);
    _spinBoxes[1]->setValue(value.y);

    return true;
}


KnobObject* KnobVector2i::creator(Sww::Core::PlugBase *plug ,QWidget *parent)
{
    return new KnobVector2i(plug,parent);
}

void KnobVector2i::setFromSpinbox(int target)
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

void KnobVector2i::x_editingFinished()
{
    setFromSpinbox(0);
}
void KnobVector2i::y_editingFinished()
{
    setFromSpinbox(1);
}
//void KnobVector2i::z_editingFinished()
//{
//    setFromSpinbox(2);
//}

void KnobVector2i::updateKnob()
{
    Imath::V2i value;
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        const Imath::V2i& valueTmp = _plug->getConstInputValue(&ok);
        if(!ok){
            SWW_ERR("postChangeState failed. " << _plug->getDebugName());
            return;
        }
        value = valueTmp;
    }

    _spinBoxes[0]->setValue(value.x);
    _spinBoxes[1]->setValue(value.y);
    //_spinBoxes[2]->setValue(value.z);


    if(_plug->prevCount() || _plug->isOutput())
    {
        for(int i = 0;i < 2;i++){
            _spinBoxes[i]->setEnabled(false);
        }
    }else  {
        for(int i = 0;i < 2;i++){
            _spinBoxes[i]->setEnabled(true);
        }
    }

}


