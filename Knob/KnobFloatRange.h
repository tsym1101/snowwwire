#ifndef KnobFloatRange_H
#define KnobFloatRange_H


//namespace Ui {
//class KnobFloatRange;
//}

//class KnobFloatRange : public QWidget
//{
//    Q_OBJECT
    
//public:
//    explicit KnobFloatRange(QWidget *parent = 0);
//    ~KnobFloatRange();
    
//private:
//    Ui::KnobFloatRange *ui;
//};




#include "Knob/KnobObject.h"
#include "Core/SwwMacro.h"
#include "KnobAPI.h"

namespace Ui {
class KnobFloatRange;
}
namespace SwwKnob {
class FloatRangeSlider;
}

namespace Sww::Core{
class PlugBase;
class V2fPlug;
}

class SWWKNOBSHARED_EXPORT KnobFloatRange : public KnobObject
{
    Q_OBJECT
public:
    explicit KnobFloatRange(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    ~KnobFloatRange();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    bool initialize();

public Q_SLOTS:
    virtual void updateKnob();

private Q_SLOTS:
    void slider_valueChanged(float v0, float v1);

private:
    Ui::KnobFloatRange *ui;
    SwwKnob::FloatRangeSlider* _slider;
    Sww::Core::V2fPlug* _plug;

};


#endif // KnobFloatRange_H
