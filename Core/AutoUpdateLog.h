#ifndef AUTOUPDATELOG_H
#define AUTOUPDATELOG_H

#include "Core/PlugBase.h"
#include <QStringList>
#include <QDebug>

SWW_CORE_NAMESPACE_BEGIN

class AutoUpdateLog
{
public:
    static void clearAutoUpdatelog();
    static void addAutoUpdatelog(const QString& log){_autoUpdatelog << log;}
    static void countAutoUpdateExec(){_autoUpdateExecCount++;}
    static void clearAutoUpdateExecCount(){_autoUpdateExecCount = 0;}
    static void printAutoUpdateLog(){Q_FOREACH(QString str,_autoUpdatelog){qDebug() << str;}}
    static size_t getAutoUpdateExecCount(){return _autoUpdateExecCount;}
    static void startExecLog(PlugBase *p);
    static void endExecLog(PlugBase *p, bool result);

private:
    AutoUpdateLog(){}
    static  QStringList _autoUpdatelog;
    static size_t _autoUpdateExecCount;
};

SWW_CORE_NAMESPACE_END

#endif // AUTOUPDATELOG_H
