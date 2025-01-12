#ifndef KnobMultiLineEdit_H
#define KnobMultiLineEdit_H

#include "Knob/KnobObject.h"
#include "Core/SwwMacro.h"
#include "KnobAPI.h"

namespace Ui {
class KnobMultiLineEdit;
}

namespace Sww::Core{
class PlugBase;
class StringArrayPlug;
}

class SWWKNOBSHARED_EXPORT KnobMultiLineEdit : public KnobObject
{
    Q_OBJECT

public:
    explicit KnobMultiLineEdit(Sww::Core::PlugBase *plug,QWidget *parent = 0);
    ~KnobMultiLineEdit();
    static KnobObject* creator(Sww::Core::PlugBase *plug ,QWidget *parent = 0);
    bool initialize();


private Q_SLOTS:
    virtual void updateKnob();
    void on_textEdit_editingFinished();

private:
    Ui::KnobMultiLineEdit *ui;
    Sww::Core::StringArrayPlug* _plug;

};

#endif // KnobMultiLineEdit_H
