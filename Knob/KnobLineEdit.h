#ifndef KnobLineEdit_H
#define KnobLineEdit_H

#include "Knob/KnobObject.h"
#include "Core/SwwMacro.h"
#include "KnobAPI.h"
#include <QLineEdit>
#include <QLabel>

namespace Sww::Core{
class PlugBase;
class StringPlug;
}

class SWWKNOBSHARED_EXPORT KnobLineEdit : public KnobObject
{
    Q_OBJECT
public:
    explicit KnobLineEdit(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    ~KnobLineEdit();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    bool initialize();

public Q_SLOTS:
    virtual void updateKnob();

private Q_SLOTS:
    void lineEdit_editingFinished();

private:
    //QLabel* _labelPlugName;
    QLineEdit* _lineEdit;
    Sww::Core::StringPlug* _plug;

//    SWW_SIMPLE_MEMBER(float,maxValue)
//    SWW_SIMPLE_MEMBER(float,minValue)

};

#endif // KnobLineEdit_H
