#ifndef SWWGEOM_GLOBAL_H
#define SWWGEOM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SWWGEOM_LIBRARY)
#  define SWWGEOMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SWWGEOMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SWWGEOM_GLOBAL_H
