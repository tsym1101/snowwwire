#ifndef SWWVERSION_H
#define SWWVERSION_H
#include <string>
#include "Core/CoreAPI.h"

SWW_CORE_NAMESPACE_BEGIN

int SWWCORESHARED_EXPORT SwwMajorVersion();
int SWWCORESHARED_EXPORT SwwMinorVersion();
int SWWCORESHARED_EXPORT SwwPatchVersion();
std::string SWWCORESHARED_EXPORT SwwVersionStr();
std::string SWWCORESHARED_EXPORT SwwMajorVersionStr();
std::string SWWCORESHARED_EXPORT SwwMinorVersionStr();
std::string SWWCORESHARED_EXPORT SwwPatchVersionStr();

SWW_CORE_NAMESPACE_END

#endif // SWWVERSION_H
