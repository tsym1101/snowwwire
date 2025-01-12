#ifndef ADDDYNAMICPLUGWIDGET_H
#define ADDDYNAMICPLUGWIDGET_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif

namespace Sww::Core{
class Node;
}

namespace Ui {
class AddDynamicPlugWidget;
}

class AddDynamicPlugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddDynamicPlugWidget(Sww::Core::Node* node,QWidget *parent = 0);
    ~AddDynamicPlugWidget();
private slots:
    void on_pushButtonOk_clicked();
    void on_pushButtonCencel_clicked();
private:
    Ui::AddDynamicPlugWidget *ui;
    Sww::Core::Node* _node;
};

#endif // ADDDYNAMICPLUGWIDGET_H
