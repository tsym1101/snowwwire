#ifndef KNOBTF_H
#define KNOBTF_H

//#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
//#include <QWidget>
//#else
//#include <QtWidgets/QtWidgets>
//#endif

#include "Knob/KnobObject.h"
#include "Knob/KnobAPI.h"

class QWidget;

namespace Ui {
class KnobTF;
}

namespace SwwGeom{
class TransFuncMappingCanvas;
class TransferFunctionPlug;
}

class SWWKNOBSHARED_EXPORT KnobTF : public KnobObject
{
    Q_OBJECT
    
public:
    explicit KnobTF(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    ~KnobTF();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    bool initialize();

public Q_SLOTS:
    virtual void updateKnob();
private Q_SLOTS:
    void tf_modified();
    void tf_reset();
    void on_spinBoxResolution_editingFinished();
    //void on_comboBoxMode_activated(int index);
    void setToGui();

private:
    Ui::KnobTF *ui;
    SwwGeom::TransFuncMappingCanvas* _canvas;
    SwwGeom::TransferFunctionPlug* _plug;
};

#endif // KNOBTF_H
