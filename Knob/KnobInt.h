#ifndef KNOBINT_H
#define KNOBINT_H

#include "Knob/KnobObject.h"
#include "Core/SwwMacro.h"
#include "KnobAPI.h"

namespace Ui {
class KnobInt;
}

namespace  Sww::Core {
class IntPlug;
}

class SWWKNOBSHARED_EXPORT KnobInt : public KnobObject
{
    Q_OBJECT
    
public:
    explicit KnobInt(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    ~KnobInt();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    bool initialize();
public Q_SLOTS:
   virtual void updateKnob();



private Q_SLOTS:
   //void on_spinBox_valueChanged(int arg1);
   void on_spinBox_editingFinished();

private:
    Ui::KnobInt *ui;

     Sww::Core::IntPlug* _plug;
     int _oldValue;

//    bool _firstValueChanged;
//    bool _acceptChanged; //基本的にはtrueにしておく。falseにしてからsetValueすればon_doubleSpinBox_valueChangedをブロックする



};

#endif // KNOBINT_H
