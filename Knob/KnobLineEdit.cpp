#include "KnobLineEdit.h"

#include <Core/Node.h>
#include "Core/PlugType.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>

KnobLineEdit::KnobLineEdit(Sww::Core::PlugBase *plug, QWidget *parent):KnobObject(plug,parent)
{
    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::STRING,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::StringPlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");

    QHBoxLayout* layout = new QHBoxLayout();

    layout->setContentsMargins(0,0,0,0);

    _lineEdit = new QLineEdit();
    _lineEdit->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    layout->addWidget(_lineEdit);

    setLayout(layout);

    if(plug->isOutput())_lineEdit->setEnabled(false);//outputは編集できない
    connect(_lineEdit,SIGNAL(editingFinished()),this,SLOT(lineEdit_editingFinished()));
}

KnobLineEdit::~KnobLineEdit()
{
}

void KnobLineEdit::updateKnob()
{
    Q_ASSERT(!_plug->dataIsNull());

    std::string value;
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

    if(_plug->prevCount() || _plug->isOutput()) _lineEdit->setEnabled(false);
    else  _lineEdit->setEnabled(true);

    _lineEdit->setText(value.c_str());
}

bool KnobLineEdit::initialize()
{
    if(_plug->prevCount())
    {
        _lineEdit->setEnabled(false);
    }

    std::string value;
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
    _lineEdit->setText(value.c_str());

    return true;
}

KnobObject* KnobLineEdit::creator(Sww::Core::PlugBase *plug ,QWidget *parent)
{
    return new KnobLineEdit(plug,parent);
}

void KnobLineEdit::lineEdit_editingFinished()
{

    std::string value = _lineEdit->text().toStdString();
    SWW_VAL_VOID(_plug,currentValue);
    if(value == currentValue){
        return;
    }
    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    if(!_plug->setInputValue(value)){

    }

    cmd->push();
}
