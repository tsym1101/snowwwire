#ifndef KNOBVECTOR2F_H
#define KNOBVECTOR2F_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif

#include "Knob/KnobObject.h"
#include "KnobAPI.h"

namespace Ui {
class KnobVector2f;
}
namespace Sww::Core{
class PlugBase;
class V2fPlug;
}

class CustomSpinBox;

class SWWKNOBSHARED_EXPORT KnobVector2f : public KnobObject
{
    Q_OBJECT

public:
    KnobVector2f(Sww::Core::PlugBase *plug,QWidget *parent = 0);
    ~KnobVector2f();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    bool initialize();
public Q_SLOTS:
    virtual void updateKnob();

private Q_SLOTS:
    void x_editingFinished();
    void y_editingFinished();

private:
    Ui::KnobVector2f *ui;
    CustomSpinBox* _spinBoxes[2];
    void setFromSpinbox(int target);
    Sww::Core::V2fPlug* _plug;
};

#endif // KNOBVECTOR3F_H
