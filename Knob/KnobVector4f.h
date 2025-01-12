#ifndef KNOBVECTOR4F_H
#define KNOBVECTOR4F_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif

#include "Knob/KnobObject.h"
#include "KnobAPI.h"

namespace Ui {
class KnobVector4f;
}
namespace Sww::Core{
class PlugBase;
class V4fPlug;
}

class CustomSpinBox;

class SWWKNOBSHARED_EXPORT KnobVector4f : public KnobObject
{
    Q_OBJECT
public:
    KnobVector4f(Sww::Core::PlugBase *plug,QWidget *parent = 0);
    ~KnobVector4f();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    bool initialize();
public Q_SLOTS:
    virtual void updateKnob();
private Q_SLOTS:
    void x_editingFinished();
    void y_editingFinished();
    void z_editingFinished();
    void w_editingFinished();
private:
    Ui::KnobVector4f *ui;
    CustomSpinBox* _spinBoxes[4];
    void setFromSpinbox(int target);
    Sww::Core::V4fPlug* _plug;
};

#endif // KNOBVECTOR4F_H
