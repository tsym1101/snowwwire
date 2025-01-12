#ifndef SWWOPENGLAPI_H
#define SWWOPENGLAPI_H

#include <QtCore/qglobal.h>

#if defined(SWWOPENGL_LIBRARY)
#  define SWWOPENGLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SWWOPENGLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SWWOPENGLAPI_H
