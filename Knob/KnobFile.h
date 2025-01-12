#ifndef KNOBFILE_H
#define KNOBFILE_H

#include "Knob/KnobObject.h"
#include "Core/SwwMacro.h"
#include "KnobAPI.h"

namespace Ui {
class KnobFile;
}

namespace Sww::Core{
class PlugBase;
class FilePlug;
}

class SWWKNOBSHARED_EXPORT KnobFile : public KnobObject
{
    Q_OBJECT

    //friend class KnobMultiFile;

public:
    explicit KnobFile(Sww::Core::PlugBase *plug,QWidget *parent = 0);
    ~KnobFile();
    static KnobObject* creator(Sww::Core::PlugBase *plug ,QWidget *parent = 0);
    bool initialize();

    static  QHash<QString,QString>& getLastOpenDialogPath(){return _lastOpenDialogPath;}
    static  QHash<QString,QString>& getLastSaveDialogPath(){return _lastSaveDialogPath;}

    static  QString getLastOpenDialogPath(const QString& currentPathKey){
        return _lastOpenDialogPath[currentPathKey];
    }
    static  void setLastOpenDialogPath(const QString& currentPathKey,const QString& lastPath){
        _lastOpenDialogPath[currentPathKey] = lastPath;
    }


private Q_SLOTS:
    void on_pushButtonOpenImage_clicked();
    void on_lineEditFilename_editingFinished();
    virtual void updateKnob();

    //void on_checkBoxAsSequence_clicked(bool checked);
private:
    Ui::KnobFile *ui;
    Sww::Core::FilePlug* _plug;

    static QHash<QString,QString> _lastOpenDialogPath;
    static QHash<QString,QString> _lastSaveDialogPath;
};


#endif // KNOBFILE_H
