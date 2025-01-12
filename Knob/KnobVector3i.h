#ifndef KNOBVECTOR3i_H
#define KNOBVECTOR3i_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#include <QSpinBox>
#else
#include <QtWidgets/QtWidgets>
#include <QtWidgets/QSpinBox>
#endif


#include "Knob/KnobObject.h"
#include "KnobAPI.h"

namespace Ui {
class KnobVector3i;
}
namespace Sww::Core{
class PlugBase;
class V3iPlug;
}

class CustomSpinBox;

class SWWKNOBSHARED_EXPORT KnobVector3i : public KnobObject
{
    Q_OBJECT

public:
    KnobVector3i(Sww::Core::PlugBase *plug,QWidget *parent = 0);
    ~KnobVector3i();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    bool initialize();
public Q_SLOTS:
    virtual void updateKnob();

private Q_SLOTS:
    void x_editingFinished();
    void y_editingFinished();
    void z_editingFinished();

private:
    Ui::KnobVector3i *ui;
    QSpinBox* _spinBoxes[3];
    void setFromSpinbox(int target);
    Sww::Core::V3iPlug* _plug;
};

#endif // KNOBVECTOR3i_H
