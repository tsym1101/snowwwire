#ifndef SWWKNOBAPI_H
#define SWWKNOBAPI_H

#include <QtCore/qglobal.h>

#if defined(SWWKNOB_LIBRARY)
#  define SWWKNOBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SWWKNOBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SWWKNOBAPI_H
