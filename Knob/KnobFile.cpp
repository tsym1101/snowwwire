#include "KnobFile.h"
#include "ui_KnobFile.h"

#include <Core/Node.h>
#include "Core/PlugType.h"

#include <QFileDialog>
#include <QDir>

//#include "Util/IOUtility.h"
#include <QApplication>

QHash<QString,QString> KnobFile::_lastOpenDialogPath;
QHash<QString,QString> KnobFile::_lastSaveDialogPath;

KnobFile::KnobFile(Sww::Core::PlugBase *plug, QWidget *parent) :KnobObject(plug,parent),ui(new Ui::KnobFile)
{
    Q_ASSERT_X(!plug->isOutput(),plug->getDebugName().c_str(),"knobFile must be input.");

    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::STRING,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::FilePlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");

}

KnobFile::~KnobFile()
{
    delete ui;
}

bool KnobFile::initialize()
{

    bool ok;
    const std::string& value = _plug->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("getInputValue() failed." << _plug->getDebugName());
        return false;
    }

    ui->lineEditFilename->setText(value.c_str());

    return true;
}

KnobObject* KnobFile::creator(Sww::Core::PlugBase *plug , QWidget *parent)
{
    return new KnobFile(plug,parent);
}

void KnobFile::on_pushButtonOpenImage_clicked()
{
    QString fileName;

    QString currentPathKey = _plug->getExtensionString();
    currentPathKey = currentPathKey.split("(").first();
    currentPathKey = currentPathKey.remove("\"");
    currentPathKey = currentPathKey.trimmed();

    if(_plug->getMode() == Sww::Core::FilePlug::kInputFile)
    {
        QString currentPath = _lastOpenDialogPath[currentPathKey];

        fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                currentPath, //dialog path
                                                _plug->getExtensionString());

        if (fileName.isEmpty())return;
        QString lastPath = QFileInfo(fileName).dir().path();
        _lastOpenDialogPath[currentPathKey] = lastPath;

    }else if(_plug->getMode() == Sww::Core::FilePlug::kOutputFile)
    {
        QString currentPath = _lastSaveDialogPath[currentPathKey];
        fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                currentPath, //dialog path
                                                _plug->getExtensionString());
        if (fileName.isEmpty())return;
        QString lastPath = QFileInfo(fileName).dir().path();
        _lastSaveDialogPath[currentPathKey] = lastPath;

    }else if(_plug->getMode() == Sww::Core::FilePlug::kInputDir){

        QString currentPath = _lastOpenDialogPath[currentPathKey];

        fileName = QFileDialog::getExistingDirectory(this,tr("Open Dir"),currentPath);

        if (fileName.isEmpty())return;

        QString lastPath = QFileInfo(fileName).dir().path();
        _lastOpenDialogPath[currentPathKey] = lastPath;
    }

    Qt::KeyboardModifiers mod = QApplication::keyboardModifiers();
    if(mod == Qt::ControlModifier)
    {
        fileName = _plug->replace(fileName);
    }

    std::string value = fileName.toStdString();

    SWW_VAL_VOID(_plug,currentValue);
    if(value == currentValue){
        return;
    }
    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    if(!_plug->setInputValue(value))
    {
        SWW_ERR("setInputValue() failed." << _plug->getDebugName());
    }

    cmd->push();

    ui->lineEditFilename->setText(value.c_str());

}

void KnobFile::on_lineEditFilename_editingFinished()
{
    QString fileName = ui->lineEditFilename->text();
    std::string value = fileName.toStdString();

    SWW_VAL_VOID(_plug,currentValue);
    if(value == currentValue){
        return;
    }
    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    if(!_plug->setInputValue(value))
    {
        SWW_ERR("setInputValue() failed." << _plug->getDebugName());
    }

    cmd->push();
}

void KnobFile::updateKnob()
{
    Q_ASSERT(!_plug->dataIsNull());

    bool ok;
    const std::string& value = _plug->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("postChangeState failed. " << _plug->getDebugName());
        return;
    }

    if(_plug->prevCount()) {
        ui->lineEditFilename->setEnabled(false);
        ui->pushButtonOpenImage ->setEnabled(false);
    }
    else{
        ui->lineEditFilename->setEnabled(true);
        ui->pushButtonOpenImage ->setEnabled(true);
    }

    ui->lineEditFilename->setText(value.c_str());

}

