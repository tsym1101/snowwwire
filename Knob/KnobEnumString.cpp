#include "KnobEnumString.h"
#include "ui_KnobEnumString.h"
#include "Core/PlugType.h"

KnobEnumString::KnobEnumString(Sww::Core::PlugBase *plug, QWidget *parent):KnobObject(plug,parent),
    ui(new Ui::KnobEnumString)
{
    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::STRING,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::EnumStringPlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");


    QObject::connect(_plug,SIGNAL(listChanged()),this,SLOT(listChanged()));
}

KnobEnumString::~KnobEnumString()
{
    delete ui;
}

bool KnobEnumString::initialize()
{
    if(_plug->prevCount())
    {
        ui->comboBox->setEnabled(false);
    }

    ui->comboBox->addItems(_plug->getList());

    auto value = _plug->initialValue();
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

    setStringToComboBox(value);

    return true;
}

KnobObject* KnobEnumString::creator(Sww::Core::PlugBase *plug, QWidget *parent)
{
    return new KnobEnumString(plug,parent);
}

void KnobEnumString::updateKnob()
{
    auto value= _plug->initialValue();
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

    setStringToComboBox(value);
}

void KnobEnumString::on_comboBox_activated(const QString &arg1)
{
    SWW_VAL_VOID(_plug,value);

    if(value == arg1.toStdString()){
        return;
    }

    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    if(!_plug->setInputValue(arg1.toStdString()))
    {
        SWW_ERR("setInputValue failed. " << _plug->getDebugName());
    }

    cmd->push();
}

void KnobEnumString::listChanged(){
    ui->comboBox->clear();
    ui->comboBox->addItems(_plug->getList());

    auto value = _plug->initialValue();
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        value = _plug->getConstInputValue(&ok);
        if(!ok){
            SWW_ERR("getInputValue() failed." << _plug->getDebugName());
            return;
        }
    }

    setStringToComboBox(value);
}


void KnobEnumString::setStringToComboBox(const std::string& value){
    QString valueQt = value.c_str();
    int index = ui->comboBox->findText(valueQt);
    if(index == -1){
        ui->comboBox->setCurrentIndex(0);
        QString str = ui->comboBox->currentText();
        if(!_plug->setInputValue(str.toStdString()))
        {
            SWW_ERR("setInputValue failed. " << _plug->getDebugName());
        }
    }
    else(ui->comboBox->setCurrentIndex(index));
}
