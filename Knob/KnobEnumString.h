#ifndef KNOBENUMSTRING_H
#define KNOBENUMSTRING_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif

#include "Knob/KnobObject.h"
#include "Knob/KnobAPI.h"

namespace Ui {
class KnobEnum;
}

namespace Sww::Core{
class PlugBase;
class EnumStringPlug;
}


namespace Ui {
class KnobEnumString;
}

class SWWKNOBSHARED_EXPORT KnobEnumString : public KnobObject
{
    Q_OBJECT

public:
    explicit KnobEnumString(Sww::Core::PlugBase *plug, QWidget *parent = 0);
    ~KnobEnumString();
    static KnobObject* creator(Sww::Core::PlugBase *plug, QWidget *parent = 0);
public Q_SLOTS:
    virtual void updateKnob();
    bool initialize();
    void listChanged();


private Q_SLOTS:
    void on_comboBox_activated(const QString &arg1);

private:
    Ui::KnobEnumString *ui;
    Sww::Core::EnumStringPlug* _plug;

    void setStringToComboBox(const std::string &value);

};

#endif // KNOBENUMSTRING_H
