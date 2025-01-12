#ifndef KNOBFLOAT_H
#define KNOBFLOAT_H

#include "Knob/KnobObject.h"
#include "Core/SwwMacro.h"
#include "KnobAPI.h"

namespace Ui {
class KnobFloat;
}
class CustomSpinBox;

namespace Sww::Core{
class PlugBase;
class FloatPlug;
}

class SWWKNOBSHARED_EXPORT KnobFloat : public KnobObject
{
    Q_OBJECT
public:
    explicit KnobFloat(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    ~KnobFloat();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    bool initialize();

public Q_SLOTS:
    virtual void updateKnob();

private Q_SLOTS:
    void spinBox_editingFinished();

private:
    Ui::KnobFloat *ui;
    CustomSpinBox* _spinBox;
    Sww::Core::FloatPlug* _plug;

};

#endif // KNOBFLOAT_H
