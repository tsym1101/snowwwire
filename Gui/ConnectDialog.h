#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif

namespace Sww::Core{
class PlugBase;
}

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectDialog(Sww::Core::PlugBase* p,QWidget *parent = 0);
    ~ConnectDialog();

private slots:
    void on_pushButtonConnect_clicked();

private:
    Ui::ConnectDialog *ui;
    Sww::Core::PlugBase* _plug;
};

#endif // CONNECTDIALOG_H
