#ifndef SWWLOG_GLOBAL_H
#define SWWLOG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SWWLOG_LIBRARY)
#  define SWWLOGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SWWLOGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SWWLOG_GLOBAL_H
