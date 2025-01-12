#ifndef KNOBVECTOR2i_H
#define KNOBVECTOR2i_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif
#include <QSpinBox>

#include "Knob/KnobObject.h"
#include "KnobAPI.h"

namespace Ui {
class KnobVector2i;
}
namespace Sww::Core{
class PlugBase;
class V2iPlug;
}

class CustomSpinBox;

class SWWKNOBSHARED_EXPORT KnobVector2i : public KnobObject
{
    Q_OBJECT

public:
    KnobVector2i(Sww::Core::PlugBase *plug,QWidget *parent = 0);
    ~KnobVector2i();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    bool initialize();
public Q_SLOTS:
    virtual void updateKnob();

private Q_SLOTS:
    void x_editingFinished();
    void y_editingFinished();
    //void z_editingFinished();

private:
    Ui::KnobVector2i *ui;
    QSpinBox* _spinBoxes[2];
    void setFromSpinbox(int target);
    Sww::Core::V2iPlug* _plug;
};

#endif // KNOBVECTOR2i_H
