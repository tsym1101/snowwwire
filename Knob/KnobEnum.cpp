#include "Knob/KnobEnum.h"
#include "ui_KnobEnum.h"

#include "Core/PlugType.h"

KnobEnum::KnobEnum(Sww::Core::PlugBase *plug, QWidget *parent):KnobObject(plug,parent),
    ui(new Ui::KnobEnum)
{
    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::INT,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::EnumPlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");
}

KnobEnum::~KnobEnum()
{
    delete ui;
}

bool KnobEnum::initialize()
{
    if(_plug->prevCount())
    {
        ui->comboBox->setEnabled(false);
    }

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

    Q_ASSERT_X(value < _plug->enumCount(),_plug->getDebugName().c_str(),"out of index.");

    ui->comboBox->addItems(_plug->getList());
    ui->comboBox->setCurrentIndex(value);

    return true;
}

KnobObject* KnobEnum::creator(Sww::Core::PlugBase *plug, QWidget *parent)
{
    //SWW_DEBUG("create KnobEnum.");
    return new KnobEnum(plug,parent);
}

void KnobEnum::updateKnob()
{
    int value;
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        value = _plug->getConstInputValue(&ok);
        if(!ok)return;
    }

    if(_plug->prevCount() || _plug->isOutput()) ui->comboBox->setEnabled(false);
    else  ui->comboBox->setEnabled(true);

    ui->comboBox->setCurrentIndex(value);
}


void KnobEnum::on_comboBox_activated(int index)
{
    SWW_VAL_VOID(_plug,value);

    if(value == index){
        return;
    }

    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    if(!_plug->setInputValue(index))
    {
        SWW_ERR("setInputValue failed. " << _plug->getDebugName());
    }

    cmd->push();
}
