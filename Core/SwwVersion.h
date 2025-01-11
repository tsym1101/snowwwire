#ifndef SWWVERSION_H
#define SWWVERSION_H
#include <string>

#include "Core/CoreAPI.h"

SWW_CORE_NAMESPACE_BEGIN

int SWWCORESHARED_EXPORT dsMajorVersion();
int SWWCORESHARED_EXPORT dsMinorVersion();
int SWWCORESHARED_EXPORT dsPatchVersion();
std::string SWWCORESHARED_EXPORT dsVersionStr();
std::string SWWCORESHARED_EXPORT dsMajorVersionStr();
std::string SWWCORESHARED_EXPORT dsMinorVersionStr();
std::string SWWCORESHARED_EXPORT dsPatchVersionStr();

SWW_CORE_NAMESPACE_END

#endif // SWWVERSION_H
