#ifndef KNOBMATRIX44F_H
#define KNOBMATRIX44F_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif
#include <ImathMatrix.h>
#include "Knob/KnobObject.h"
#include "KnobAPI.h"

namespace Ui {
class KnobMatrix44f;
}

namespace Sww::Core{
class PlugBase;
class M44fPlug;
}

class CustomSpinBox;

class SWWKNOBSHARED_EXPORT KnobMatrix44f : public KnobObject
{
    Q_OBJECT

public:
    KnobMatrix44f(Sww::Core::PlugBase *plug,QWidget *parent = 0);
    ~KnobMatrix44f();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);

    bool initialize();
public Q_SLOTS:
    virtual void updateKnob();

private Q_SLOTS:
    void m00_editingFinished(){setFromSpinbox(0,0);}
    void m01_editingFinished(){setFromSpinbox(0,1);}
    void m02_editingFinished(){setFromSpinbox(0,2);}
    void m03_editingFinished(){setFromSpinbox(0,3);}
    void m10_editingFinished(){setFromSpinbox(1,0);}
    void m11_editingFinished(){setFromSpinbox(1,1);}
    void m12_editingFinished(){setFromSpinbox(1,2);}
    void m13_editingFinished(){setFromSpinbox(1,3);}
    void m20_editingFinished(){setFromSpinbox(2,0);}
    void m21_editingFinished(){setFromSpinbox(2,1);}
    void m22_editingFinished(){setFromSpinbox(2,2);}
    void m23_editingFinished(){setFromSpinbox(2,3);}
    void m30_editingFinished(){setFromSpinbox(3,0);}
    void m31_editingFinished(){setFromSpinbox(3,1);}
    void m32_editingFinished(){setFromSpinbox(3,2);}
    void m33_editingFinished(){setFromSpinbox(3,3);}

private:
    Ui::KnobMatrix44f *ui;
    CustomSpinBox* _spinBoxes[4][4];
    void setFromSpinbox(int row, int col);
    Sww::Core::M44fPlug* _plug;

};

#endif // KNOBMATRIX44F_H
