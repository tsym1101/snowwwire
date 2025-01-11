#ifndef SWWCOREAPIDEFINE_H
#define SWWCOREAPIDEFINE_H

#include "core/SwwNsCore.h"
#include <QtCore/qglobal.h>

#if defined(SWWCORE_LIBRARY)
#  define SWWCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define SWWCORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SWWCOREAPIDEFINE_H
