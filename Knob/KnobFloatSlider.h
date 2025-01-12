#ifndef KNOBFLOATSLIDER_H
#define KNOBFLOATSLIDER_H

#include "Knob/KnobObject.h"
#include "Core/SwwMacro.h"
#include "KnobAPI.h"

namespace Ui {
class KnobFloatSlider;
}

class CustomSpinBox;

namespace Sww::Core{
class PlugBase;
class FloatPlug;
}
class SWWKNOBSHARED_EXPORT KnobFloatSlider : public KnobObject
{
    Q_OBJECT
public:
    explicit KnobFloatSlider(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    ~KnobFloatSlider();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    bool initialize();

Q_SIGNALS:
    //void doubleValueChanged(double value);

public Q_SLOTS:
    virtual void updateKnob();
    void notifyValueChanged(int value);

private Q_SLOTS:
    void spinBox_editingFinished();
    void on_horizontalSlider_sliderPressed();

    void on_horizontalSlider_sliderReleased();

private:
    Ui::KnobFloatSlider *ui;
    CustomSpinBox* _spinBox;
    Sww::Core::FloatPlug* _plug;

    bool _setValueAlreadyCalled; //スピンボックス入力後にスライダーのvalueChangedが呼ばれないようにする

    SwwKnob::UndoCommand* _cmdTmp;
};

#endif // KNOBFLOATSLIDER_H
