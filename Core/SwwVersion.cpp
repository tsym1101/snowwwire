#include "SwwVersion.h"
#include <QString>

SWW_CORE_NAMESPACE_BEGIN

int SwwMajorVersion(){return 0;}
int SwwMinorVersion(){return 1;}
int SwwPatchVersion(){return 0;}

std::string SwwVersionStr(){
    std::string str;
    str =( QString::number(SwwMajorVersion()) + "." + QString::number(SwwMinorVersion()) + "."
            + QString::number(SwwPatchVersion()) ).toStdString();
    return str;
}

std::string SwwMajorVersionStr(){return QString::number(SwwMajorVersion()).toStdString();}
std::string SwwMinorVersionStr(){return QString::number(SwwMinorVersion()).toStdString();}
std::string SwwPatchVersionStr(){return QString::number(SwwPatchVersion()).toStdString();}

SWW_CORE_NAMESPACE_END
