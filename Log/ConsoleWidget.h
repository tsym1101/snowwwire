#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif
#else
#include <QtWidgets/QtWidgets>
#endif


#include "LogApi.h"

namespace Ui {
class  ConsoleWidget;
}

class SWWLOGSHARED_EXPORT ConsoleWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ConsoleWidget(QWidget *parent = 0);
    ~ConsoleWidget();



    
public Q_SLOTS:
    void showContextMenu(const QPoint &pt);

//    void logErr(const std::string& msg);
//    void logWarn(const std::string& msg);
//    void logInfo(const std::string& msg);
//    void logDebug(const std::string& msg);
//    void logDebugValue(const std::string& msg);

    void logErr(const QString& msg);
    void logWarn(const QString& msg);
    void logInfo(const QString& msg);
    void logDebug(const QString& msg);
    void logDebugValue(const QString& msg);

private:
    Ui::ConsoleWidget *ui;
    bool _autoScroll;

    QAction* _clearAction;
    QAction* _disableAction;

};

#endif // CONSOLEWIDGET_H
