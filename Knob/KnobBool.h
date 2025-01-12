#ifndef KNOBBOOL_H
#define KNOBBOOL_H

#include "Knob/KnobObject.h"
#include "Core/SwwMacro.h"
#include "KnobAPI.h"

namespace Ui {
class KnobBool;
}

namespace Sww::Core{
class PlugBase;
class BoolPlug;
}


class SWWKNOBSHARED_EXPORT KnobBool :public KnobObject
{
    Q_OBJECT

public:
    explicit KnobBool(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    ~KnobBool();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    virtual bool needNameTag()const{return false;}

public Q_SLOTS:
   virtual void updateKnob();
   bool initialize();

private Q_SLOTS:
    void on_checkBox_clicked(bool checked);

private:
    Ui::KnobBool *ui;
    Sww::Core::BoolPlug* _plug;

};

#endif // KNOBBOOL_H
