#ifndef KNOBMULTIFILE_H
#define KNOBMULTIFILE_H

#include "Knob/KnobObject.h"
#include "Core/SwwMacro.h"
#include "KnobAPI.h"

namespace Ui {
class KnobMultiFile;
}

namespace Sww::Core{
class PlugBase;
class MultiFilePlug;
}

class SWWKNOBSHARED_EXPORT KnobMultiFile : public KnobObject
{
    Q_OBJECT

public:
    explicit KnobMultiFile(Sww::Core::PlugBase *plug,QWidget *parent = 0);
    ~KnobMultiFile();
    static KnobObject* creator(Sww::Core::PlugBase *plug ,QWidget *parent = 0);
    bool initialize();

private Q_SLOTS:
    void on_pushButtonOpenImage_clicked();
    virtual void updateKnob();

private:
    Ui::KnobMultiFile *ui;
    Sww::Core::MultiFilePlug* _plug;
};

#endif // KNOBMULTIFILE_H
