#include "Log.h"
#include "Log/ConsoleWidget.h"
#include <iostream>

#include <QObject>

SWW_NAMESPACE_BEGIN

ConsoleWidget* Log::_consoleWidget;

const char* Log::errFlag = "ERR: ";
const char* Log::warnFlag = "WARN: ";
const char* Log::infoFlag = "INFO: ";
const char* Log::debugFlag = "DEBUG: ";
const char* Log::debugValueFlag = "VALUE: ";

LogFunction Log::_errLogFunction = Log::logErrNoGui;
LogFunction Log::_warnLogFunction = Log::logWarnNoGui;
LogFunction Log::_infoLogFunction = Log::logInfoNoGui;
LogFunction Log::_debugLogFunction = Log::logDebugNoGui;
LogFunction Log::_debugValueLogFunction = Log::logDebugValueNoGui;

void Log::init(){
    _consoleWidget = new ConsoleWidget();
}

void Log::deinit()
{
    setNoGuiMode();
}

void Log::setGuiMode(){
    Log::_errLogFunction = Log::logErrGui;
    Log::_warnLogFunction = Log::logWarnGui;
    Log::_infoLogFunction = Log::logInfoGui;
    Log::_debugLogFunction = Log::logDebugGui;
    Log::_debugValueLogFunction = Log::logDebugValueGui;
}

void Log::setNoGuiMode(){
    Log::_errLogFunction = Log::logErrNoGui;
    Log::_warnLogFunction = Log::logWarnNoGui;
    Log::_infoLogFunction = Log::logInfoNoGui;
    Log::_debugLogFunction = Log::logDebugNoGui;
    Log::_debugValueLogFunction = Log::logDebugValueNoGui;
}

void Log::logErr(const std::string& msg){

    (*_errLogFunction)(msg);
}

void Log::logWarn(const std::string& msg){
    (*_warnLogFunction)(msg);
}

void Log::logInfo(const std::string& msg){
    (*_infoLogFunction)(msg);
}

void Log::logDebug(const std::string& msg){
    (*_debugLogFunction)(msg);
}

void Log::logDebugValue(const std::string& msg){
    (*_debugValueLogFunction)(msg);
}

void Log::logErrGui(const std::string& msg){

    QMetaObject::invokeMethod(_consoleWidget,"logErr",
                              Q_ARG(QString,msg.c_str()) );
}

void Log::logWarnGui(const std::string& msg){

    QMetaObject::invokeMethod(_consoleWidget,"logWarn",
                              Q_ARG(QString,msg.c_str()) );
}

void Log::logInfoGui(const std::string& msg){

    QMetaObject::invokeMethod(_consoleWidget,"logInfo",
                              Q_ARG(QString,msg.c_str()) );
}

void Log::logDebugGui(const std::string& msg){

    QMetaObject::invokeMethod(_consoleWidget,"logDebug",
                              Q_ARG(QString,msg.c_str()) );
}

void Log::logDebugValueGui(const std::string& msg){

    QMetaObject::invokeMethod(_consoleWidget,"logDebugValue",
                              Q_ARG(QString,msg.c_str()) );
}

void Log::logErrNoGui(const std::string& msg){
    std::cout << errFlag << msg << std::endl;
}

void Log::logWarnNoGui(const std::string& msg){
    std::cout << warnFlag << msg << std::endl;
}

void Log::logInfoNoGui(const std::string& msg){
    std::cout << infoFlag << msg << std::endl;
}

void Log::logDebugNoGui(const std::string& msg){
    std::cout << debugFlag << msg << std::endl;
}

void Log::logDebugValueNoGui(const std::string& msg){
    std::cout << debugValueFlag << msg << std::endl;
}

SWW_NAMESPACE_END
