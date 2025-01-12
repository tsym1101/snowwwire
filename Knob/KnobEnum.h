#ifndef KNOBENUM_H
#define KNOBENUM_H

#include "Knob/KnobObject.h"
#include "Knob/KnobAPI.h"

namespace Ui {
class KnobEnum;
}

namespace Sww::Core{
class PlugBase;
class EnumPlug;
}

class SWWKNOBSHARED_EXPORT KnobEnum :public KnobObject
{
    Q_OBJECT

public:
    explicit KnobEnum(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    ~KnobEnum();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);
public Q_SLOTS:
    virtual void updateKnob();
    bool initialize();


private Q_SLOTS:
    void on_comboBox_activated(int index);

private:
    Ui::KnobEnum *ui;
    Sww::Core::EnumPlug* _plug;


};

#endif // KNOBENUM_H
