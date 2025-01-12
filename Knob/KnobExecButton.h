#ifndef KNOBEXECBUTTON_H
#define KNOBEXECBUTTON_H

#include "Knob/KnobObject.h"
#include "Core/SwwMacro.h"
#include "KnobAPI.h"

namespace Ui {
class KnobExecButton;
}

namespace Sww::Core {
class PlugBase;
class NullPlug;
}

class SWWKNOBSHARED_EXPORT KnobExecButton : public KnobObject
{
    Q_OBJECT
    
public:
    explicit KnobExecButton(Sww::Core::PlugBase *plug,QWidget *parent = 0);
    ~KnobExecButton();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    bool initialize();
    virtual bool needNameTag()const{return false;}
    
private Q_SLOTS:
    void on_pushButtonExec_clicked();
private:
    Ui::KnobExecButton *ui;
    Sww::Core::NullPlug* _plug;

 };

#endif // KNOBEXECBUTTON_H
