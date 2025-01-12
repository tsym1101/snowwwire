#include "KnobBool.h"
#include "ui_KnobBool.h"

#include "Core/PlugType.h"

KnobBool::KnobBool(Sww::Core::PlugBase *plug, QWidget *parent):KnobObject(plug,parent),
    ui(new Ui::KnobBool)
{
    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::BOOL,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::BoolPlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");

}

KnobBool::~KnobBool()
{
    delete ui;
}

bool KnobBool::initialize()
{
    if(_plug->prevCount())
    {
        ui->checkBox->setEnabled(false);
    }
    ui->checkBox->setText(_plug->getName().c_str());

    bool value;
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
    ui->checkBox->setChecked(value);
    return true;
}

KnobObject* KnobBool::creator(Sww::Core::PlugBase *plug, QWidget *parent)
{
    return new KnobBool(plug,parent);
}

void KnobBool::updateKnob()
{

    bool value;
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        value = _plug->getConstInputValue(&ok);
        if(!ok)return;
    }

    if(_plug->prevCount() || _plug->isOutput()) ui->checkBox->setEnabled(false);
    else  ui->checkBox->setEnabled(true);

    ui->checkBox->setChecked(value);


}


void KnobBool::on_checkBox_clicked(bool checked)
{
    SWW_VAL_VOID(_plug,value);
    //bool currentValue = ui->checkBox->isChecked();
    if(value == checked){
        return;
    }

    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    if(!_plug->setInputValue(checked))
    {
        SWW_ERR("setInputValue failed. " << _plug->getDebugName());
    }

    cmd->push();
}
