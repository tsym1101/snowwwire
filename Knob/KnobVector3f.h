#ifndef KNOBVECTOR3F_H
#define KNOBVECTOR3F_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif

#include "Knob/KnobObject.h"
#include "KnobAPI.h"

namespace Ui {
class KnobVector3f;
}
namespace Sww::Core{
class PlugBase;
class V3fPlug;
}

class CustomSpinBox;

class SWWKNOBSHARED_EXPORT KnobVector3f : public KnobObject
{
    Q_OBJECT

public:
    KnobVector3f(Sww::Core::PlugBase *plug,QWidget *parent = 0);
    ~KnobVector3f();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    bool initialize();
public Q_SLOTS:
    virtual void updateKnob();

private Q_SLOTS:
    void x_editingFinished();
    void y_editingFinished();
    void z_editingFinished();

private:
    Ui::KnobVector3f *ui;
    CustomSpinBox* _spinBoxes[3];
    void setFromSpinbox(int target);
    Sww::Core::V3fPlug* _plug;
};

#endif // KNOBVECTOR3F_H
