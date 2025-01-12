#ifndef KNOBWIDGET_H
#define KNOBWIDGET_H

#include "Gui/GuiAPI.h"
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif


namespace Ui {
class KnobWidget;
}

class SWWGUISHARED_EXPORT KnobWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KnobWidget(QWidget *parent = 0);
    ~KnobWidget();

    void clear();
    //bool isActive(QWidget* group);  //引数のwidgetが現在表示中かどうかを確認
    //void setActiveNodeItem(void *nodeItem){_activeNodeItem = nodeItem;}
    //void removeEditor(QWidget *widget);
    //void setNodeName(const std::string &name);
public Q_SLOTS:
    void addKnob(QWidget *widget);

private:
    Ui::KnobWidget *ui;
    //QWidget* _activeKnobGroup;

};


#endif // KNOBWIDGET_H
