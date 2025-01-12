#include "KnobMultiLineEdit.h"
#include "ui_KnobMultiLineEdit.h"

#include <Core/Node.h>
#include "Core/PlugType.h"

#include <QFileDialog>
#include <QDir>

//#include "Util/IOUtility.h"

#include "Knob/KnobFile.h"


KnobMultiLineEdit::KnobMultiLineEdit(Sww::Core::PlugBase *plug, QWidget *parent) :KnobObject(plug,parent),ui(new Ui::KnobMultiLineEdit)
{
    //Q_ASSERT_X(!plug->isOutput(),plug->getDebugName().c_str(),"knobFile must be input.");

    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::STRING_ARRAY,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::StringArrayPlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");

}

KnobMultiLineEdit::~KnobMultiLineEdit()
{
    delete ui;
}

bool KnobMultiLineEdit::initialize()
{
    //ui->labelPlugName->setText(_plug->getName().c_str());

    bool ok;
    //const Sww::Core::StringArrayPtr& value = _plug->getConstInputValue(&ok);
    Sww::Core::StringArrayPtr value;
    if(!_plug->isOutput()){
        value = _plug->getConstInputValue(&ok);
        if(!ok){
            SWW_ERR("getInputValue() failed." << _plug->getDebugName());
            return false;
        }
    }
    else{
        value = _plug->getConstOutputValue();
        ui->textEdit->setEnabled(false);
    }

    Q_ASSERT(value.get());

    const Sww::Core::StringArray& array = *(value.get());
    QStringList strList;
    SWW_VECTOR_FOREACH_CONST(std::string,array){
        QString str = (*i).c_str();
        strList.push_back(str);

        //ui->textEdit->addItem(str);
    };
    ui->textEdit->setText(strList.join("\n"));

    return true;
}

KnobObject* KnobMultiLineEdit::creator(Sww::Core::PlugBase *plug , QWidget *parent)
{
    return new KnobMultiLineEdit(plug,parent);
}

void KnobMultiLineEdit::updateKnob()
{
    Q_ASSERT(!_plug->dataIsNull());

    Sww::Core::StringArrayPtr value;
    if(!_plug->isOutput()){
        bool ok;
        value = _plug->getConstInputValue(&ok);
        if(!ok){
            SWW_ERR("updateKnob() failed." << _plug->getDebugName());
            return;
        }

        if(_plug->prevCount()) {
            ui->textEdit->setEnabled(false);
        }
        else{
            ui->textEdit->setEnabled(true);
        }
    }
    else{
        value = _plug->getConstOutputValue();
    }

    Q_ASSERT(value.get());

    ui->textEdit->clear();

    const Sww::Core::StringArray& array = *(value.get());
    QStringList strList;
    SWW_VECTOR_FOREACH_CONST(std::string,array){
        QString str = (*i).c_str();
        strList.push_back(str);

        //ui->textEdit->addItem(str);
    };
    ui->textEdit->setText(strList.join("\n"));


}

void KnobMultiLineEdit::on_textEdit_editingFinished(){
    QString str = ui->textEdit->toPlainText();
    QStringList strList = str.split("\n");
    Sww::Core::StringArrayPtr strArrayPtr(new Sww::Core::StringArray());

    Q_FOREACH(QString strTmp,strList){
        strArrayPtr->push_back(strTmp.toStdString());
    }

    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    _plug->setInputValue(strArrayPtr);

    cmd->push();
}

