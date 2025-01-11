#ifndef SWWUTIL_GLOBAL_H
#define SWWUTIL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SWWUTIL_LIBRARY)
#  define SWWUTILSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SWWUTILSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "SwwNsUtil.h"

#endif // SWWUTIL_GLOBAL_H
