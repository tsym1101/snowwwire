#ifndef KNOBGROUPWIDGET_H
#define KNOBGROUPWIDGET_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif
#include "KnobAPI.h"

namespace Ui {
class KnobGroupWidget;
}

class KnobBaseWidget;

class SWWKNOBSHARED_EXPORT KnobGroupWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit KnobGroupWidget(QWidget *parent = 0);
    ~KnobGroupWidget();

    void setNodeName(const QString& nodeName);
    void addKnobBase(KnobBaseWidget* knobBase);
    void removeWidget(KnobBaseWidget* knobBase);

    //void updateChildren();
private:
    Ui::KnobGroupWidget *ui;
    //QList<KnobBaseWidget*> _children;
};

#endif // KNOBGROUPWIDGET_H
