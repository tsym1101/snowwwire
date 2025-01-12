#ifndef SWWGUIAPI_H
#define SWWGUIAPI_H

#include <QtCore/qglobal.h>

#if defined(SWWGUI_LIBRARY)
#  define SWWGUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define SWWGUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SWWGUIAPI_H
