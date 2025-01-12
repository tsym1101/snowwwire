#ifndef KNOBINTSLIDER_H
#define KNOBINTSLIDER_H

#include "Knob/KnobObject.h"
#include "Core/SwwMacro.h"
#include "KnobAPI.h"

namespace Ui {
class KnobIntSlider;
}


namespace Sww::Core{
class PlugBase;
class IntPlug;
}
class SWWKNOBSHARED_EXPORT KnobIntSlider : public KnobObject
{
    Q_OBJECT
public:
    explicit KnobIntSlider(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    ~KnobIntSlider();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    bool initialize();

public Q_SLOTS:
    virtual void updateKnob();

private Q_SLOTS:
    void spinBox_editingFinished();
    void slider_valueChanged(int value);
    void on_horizontalSlider_sliderPressed();

    void on_horizontalSlider_sliderReleased();

private:
    Ui::KnobIntSlider *ui;
    Sww::Core::IntPlug* _plug;

    bool _setValueAlreadyCalled; //スピンボックス入力後にスライダーのvalueChangedが呼ばれないようにする

    SwwKnob::UndoCommand* _cmdTmp;
};

#endif // KNOBINTSLIDER_H
