#ifndef KNOBBASEWIDGET_H
#define KNOBBASEWIDGET_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif
#include "Knob/KnobAPI.h"

class KnobObject;

namespace Ui {
class KnobBaseWidget;
}

class SWWKNOBSHARED_EXPORT KnobBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KnobBaseWidget(QWidget *parent = 0);
    ~KnobBaseWidget();

    void setKnob(QWidget *knob);
    bool hasKnob(const std::string& knobName)const;

    KnobObject* knob()const{return _knob;}
private:
    Ui::KnobBaseWidget *ui;
    KnobObject* _knob;
};

#endif // KNOBBASEWIDGET_H
