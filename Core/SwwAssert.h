#ifndef SWWASSERT_H
#define SWWASSERT_H

#include <iostream>
#include "Log/Log.h"

#if 0
#ifdef QT_DEBUG
#define SWW_ERR(message)  std::cout << "ERR   : " << message << " line " << __LINE__ << " in " << __FILE__ << std::endl;
#define SWW_INF(message)  std::cout << "INF   : " << message << " line " << __LINE__ << " in " << __FILE__ << std::endl;
#define SWW_WARN(message)  std::cout << "WARN  : " << message << " line " << __LINE__ << " in " << __FILE__ << std::endl;
#define SWW_DEBUG(message) std::cout << "DEBUG : " << message << " line " << __LINE__ << " in " << __FILE__ << std::endl;
#define SWW_DEBUG_VALUE(var) std::cout << "VALUE : " << #var << "= " << var << " line " << __LINE__ << " in " << __FILE__ << std::endl;
#else
#define SWW_ERR(message)  std::cout << "ERR : " << message << " line " << __LINE__ << " in " << __FILE__ << std::endl;
#define SWW_INF(message)  std::cout << "INF : " << message << std::endl;
#define SWW_WARN(message)  std::cout << "WARN : " << message << std::endl;
#define SWW_DEBUG(message) {};
#define SWW_DEBUG_VALUE(var) {};
#endif

#else

#define SWW_ERR(message) SWW_LOG_ERR(message)
#define SWW_WARN(message) SWW_LOG_WARN(message)
#define SWW_INF(message) SWW_LOG_INFO(message)
#define SWW_DEBUG(message) SWW_LOG_DEBUG(message)
#define SWW_DEBUG_VALUE(message) SWW_LOG_DEBUG_VALUE(message)



#endif

#endif // SWWASSERT_H
