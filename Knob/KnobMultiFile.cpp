#include "KnobMultiFile.h"
#include "ui_KnobMultiFile.h"

#include <Core/Node.h>
#include "Core/PlugType.h"

#include <QFileDialog>
#include <QDir>

//#include "Util/IOUtility.h"

#include "Knob/KnobFile.h"

KnobMultiFile::KnobMultiFile(Sww::Core::PlugBase *plug, QWidget *parent) :KnobObject(plug,parent),ui(new Ui::KnobMultiFile)
{
    Q_ASSERT_X(!plug->isOutput(),plug->getDebugName().c_str(),"knobFile must be input.");

    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::STRING_ARRAY,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::MultiFilePlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");

}

KnobMultiFile::~KnobMultiFile()
{
    delete ui;
}

bool KnobMultiFile::initialize()
{
    bool ok;
    const Sww::Core::StringArrayPtr& value = _plug->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("getInputValue() failed." << _plug->getDebugName());
        return false;
    }

    Q_ASSERT(value.get());

    const Sww::Core::StringArray& array = *(value.get());
    SWW_VECTOR_FOREACH_CONST(std::string,array){
        QString str = (*i).c_str();
        ui->listWidget->addItem(str);
    };

    return true;
}

KnobObject* KnobMultiFile::creator(Sww::Core::PlugBase *plug , QWidget *parent)
{
    return new KnobMultiFile(plug,parent);
}

void KnobMultiFile::on_pushButtonOpenImage_clicked()
{
    QStringList fileNames;

    QString currentPathKey = _plug->getExtensionString();
    currentPathKey = currentPathKey.split("(").first();
    currentPathKey = currentPathKey.remove("\"");
    currentPathKey = currentPathKey.trimmed();

    if(_plug->getMode() == Sww::Core::FilePlug::kInputFile)
    {
        QString currentPath = KnobFile::getLastOpenDialogPath(currentPathKey);

        fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),
                                                  currentPath, //dialog path
                                                  _plug->getExtensionString());

        if (fileNames.isEmpty())return;
        QString lastPath = QFileInfo(fileNames[0]).dir().path();
        //KnobFile::_lastOpenDialogPath[currentPathKey] = lastPath;
        KnobFile::setLastOpenDialogPath(currentPathKey,lastPath);

    }else
    {
        Q_ASSERT_X(false,_plug->getDebugName().c_str(),"multiFile plug must be input mode;");
    }

    Sww::Core::StringArrayPtr stringArray(new Sww::Core::StringArray());

    Q_FOREACH(QString str,fileNames){
        std::string value = str.toStdString();
        stringArray->push_back(value);
    }

    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    if(!_plug->setInputValue(stringArray))
    {
        SWW_ERR("getInputValue() failed." << _plug->getDebugName());
        //return;
    }

    ui->listWidget->clear();
    ui->listWidget->addItems(fileNames);

    cmd->push();
}

void KnobMultiFile::updateKnob()
{
    Q_ASSERT(!_plug->dataIsNull());

    bool ok;
    const Sww::Core::StringArrayPtr& value = _plug->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("updateKnob() failed." << _plug->getDebugName());
        return;
    }

    if(_plug->prevCount()) {
        ui->listWidget->setEnabled(false);
        ui->pushButtonOpenImage ->setEnabled(false);
    }
    else{
        ui->listWidget->setEnabled(true);
        ui->pushButtonOpenImage ->setEnabled(true);
    }

    Q_ASSERT(value.get());

    ui->listWidget->clear();

    const Sww::Core::StringArray& array = *(value.get());
    SWW_VECTOR_FOREACH_CONST(std::string,array){
        QString str = (*i).c_str();
        ui->listWidget->addItem(str);
    };

}
