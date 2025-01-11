#ifndef SWWLOG_H
#define SWWLOG_H

#include "LogApi.h"
#include <sstream>

typedef void (*LogFunction)(const std::string&);
class ConsoleWidget;

#include "SwwNs.h"

//#define SWW_LOG_NAMESPACE_BEGIN SWW_NAMESPACE_BEGIN SWW_NAMESPACE_BEGIN_DEF(Log)
//#define SWW_LOG_NAMESPACE_END SWW_NAMESPACE_END_DEF SWW_NAMESPACE_END_DEF

SWW_NAMESPACE_BEGIN

class SWWLOGSHARED_EXPORT Log
{
    friend class MainWindow;
public:
    static void init();
    static void deinit();
    //static std::stringstream& getBuffer(){return _buffer;}

    static void logErr(const std::string& msg);
    static void logWarn(const std::string& msg);
    static void logInfo(const std::string& msg);
    static void logDebug(const std::string& msg);
    static void logDebugValue(const std::string& msg);

    static void setGuiMode();
    static void setNoGuiMode();

private:
    static ConsoleWidget* _consoleWidget;

    static LogFunction _errLogFunction;
    static LogFunction _warnLogFunction;
    static LogFunction _infoLogFunction;
    static LogFunction _debugLogFunction;
    static LogFunction _debugValueLogFunction;

    static const char* errFlag;
    static const char* warnFlag;
    static const char* infoFlag;
    static const char* debugFlag;
    static const char* debugValueFlag;

    static void logErrGui(const std::string& msg);
    static void logWarnGui(const std::string& msg);
    static void logInfoGui(const std::string& msg);
    static void logDebugGui(const std::string& msg);
    static void logDebugValueGui(const std::string& msg);
    static void logErrNoGui(const std::string& msg);
    static void logWarnNoGui(const std::string& msg);
    static void logInfoNoGui(const std::string& msg);
    static void logDebugNoGui(const std::string& msg);
    static void logDebugValueNoGui(const std::string& msg);
};

#ifdef QT_DEBUG
#define  SWW_LOG_APPENDIX "\t\t line "<<__LINE__<<" in "<<__FILE__
#else
#define  SWW_LOG_APPENDIX ""
#endif

#define SWW_LOG_ERR(message)  {std::stringstream s;s << message << SWW_LOG_APPENDIX;Sww::Log::logErr(s.str());}
#define SWW_LOG_WARN(message)  {std::stringstream s;s << message << SWW_LOG_APPENDIX;Sww::Log::logWarn(s.str());}
#define SWW_LOG_INFO(message)  {std::stringstream s;s << message << SWW_LOG_APPENDIX;Sww::Log::logInfo(s.str());}

#ifdef QT_DEBUG
#define SWW_LOG_DEBUG(message)  {std::stringstream s;s << message << SWW_LOG_APPENDIX;Sww::Log::logDebug(s.str());}
#define SWW_LOG_DEBUG_VALUE(message)  {std::stringstream s;s << #message << " = " << message << SWW_LOG_APPENDIX;Sww::Log::logDebugValue(s.str());}
#else
#define SWW_LOG_DEBUG(message)  {}
#define SWW_LOG_DEBUG_VALUE(message)  {}
#endif

SWW_NAMESPACE_END

#endif // SWWLOG_H
