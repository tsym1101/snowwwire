#include "KnobInt.h"
#include "ui_KnobInt.h"

#include "Core/PlugType.h"

KnobInt::KnobInt(Sww::Core::PlugBase *plug, QWidget *parent):KnobObject(plug,parent),_oldValue(0),
    ui(new Ui::KnobInt)
{
    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::INT,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::IntPlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");

    if(_plug->isOutput())ui->spinBox->setEnabled(false);//outputは編集できない
}

KnobInt::~KnobInt()
{
    delete ui;
}

void KnobInt::updateKnob()
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

    if(_plug->prevCount() || _plug->isOutput()) ui->spinBox->setEnabled(false);
    else  ui->spinBox->setEnabled(true);

    ui->spinBox->setValue(value);
}

bool KnobInt::initialize()
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
        if(!ok)
        {
            SWW_ERR("getInputValue() failed." << _plug->getDebugName());
            return false;
        }
    }
    ui->spinBox->setValue(value);
    ui->spinBox->setMinimum(_plug->getMin());
    ui->spinBox->setMaximum(_plug->getMax());

    return true;
}

KnobObject* KnobInt::creator(Sww::Core::PlugBase *plug , QWidget *parent)
{
    return new KnobInt(plug,parent);
}

void KnobInt::on_spinBox_editingFinished()
{
    int value = ui->spinBox->value();
    if(_oldValue == value)return;

    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    if(!_plug->setInputValue(value))
    {
        SWW_ERR("setInputValue failed. " << _plug->getDebugName());
    }
    _oldValue = value;

    cmd->push();
}
