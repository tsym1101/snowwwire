#include "SwwVersion.h"
#include <QString>

SWW_CORE_NAMESPACE_BEGIN

int dsMajorVersion(){return 3;}
int dsMinorVersion(){return 2;}
int dsPatchVersion(){return 4;}

std::string dsVersionStr(){
    std::string str;
    str =( QString::number(dsMajorVersion()) + "." + QString::number(dsMinorVersion()) + "."
            + QString::number(dsPatchVersion()) ).toStdString();
    return str;
}

std::string dsMajorVersionStr(){return QString::number(dsMajorVersion()).toStdString();}
std::string dsMinorVersionStr(){return QString::number(dsMinorVersion()).toStdString();}
std::string dsPatchVersionStr(){return QString::number(dsPatchVersion()).toStdString();}

SWW_CORE_NAMESPACE_END
