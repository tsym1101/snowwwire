#include "AutoUpdateLog.h"
#include "Core/PlugBase.h"
#include "Core/NodeBase.h"

SWW_CORE_NAMESPACE_BEGIN

QStringList AutoUpdateLog::_autoUpdatelog;
size_t AutoUpdateLog::_autoUpdateExecCount = 0;

void AutoUpdateLog::startExecLog(PlugBase* p)
{
    AutoUpdateLog::clearAutoUpdatelog();
    AutoUpdateLog::clearAutoUpdateExecCount();
    QString log = QString("exec log").leftJustified(80,'*');
    AutoUpdateLog::addAutoUpdatelog(log);
    log = "auto update triggered. " + QString(p->getDebugName().c_str());
    AutoUpdateLog::addAutoUpdatelog(log);
}

void AutoUpdateLog::endExecLog(PlugBase *p ,bool result)
{
    QString log = "exec count:" + QString::number(AutoUpdateLog::getAutoUpdateExecCount());
    AutoUpdateLog::addAutoUpdatelog(log);
    log = "auto update end. " + QString::fromStdString(p->getDebugName()) + " result:" + QString::number((int)result);
    AutoUpdateLog::addAutoUpdatelog(log);
    log = QString("").leftJustified(80,'*');
    AutoUpdateLog::addAutoUpdatelog(log);
    AutoUpdateLog::printAutoUpdateLog();
}

void AutoUpdateLog::clearAutoUpdatelog(){
    _autoUpdatelog.clear();
    _autoUpdatelog << QString();//一行あける
SWW_CORE_NAMESPACE_END


}
