#ifndef PLUGTYPEUTIL_H
#define PLUGTYPEUTIL_H

#include <QStringList>
#include "Core/CoreAPI.h"
#include <ImathMatrix.h>
#include <ImathBox.h>

SWW_CORE_NAMESPACE_BEGIN

class SWWCORESHARED_EXPORT PlugTypeUtil
{
public:
    template<class T> static QString array2String(T* array,size_t size);
    static bool strToVec2i(const QString &valueStr,Imath::V2i &v); //カンマ区切り "x、y"
    static bool strToVec3i(const QString &valueStr,Imath::V3i &v); //カンマ区切り "x、y"
    static bool strToVec2f(const QString &valueStr,Imath::V2f &v); //カンマ区切り "x、y"
    static bool strToVec3f(const QString &valueStr,Imath::V3f &v); //カンマ区切り "x、y、z"
    static bool strToVec3d(const QString &valueStr,Imath::V3d &v); //カンマ区切り "x、y、z"
    static bool strToVec4f(const QString &valueStr,Imath::V4f &v); //カンマ区切り "x、y、z,w"
    static bool strToM33f(const QString &valueStr,Imath::M33f &m);
    static bool strToM44f(const QString &valueStr,Imath::M44f &m);
    static bool strToBox3f(const QString &valueStr,Imath::Box3f &box);
};

template<class T>
QString PlugTypeUtil::array2String(T* array,size_t size)
{
    QStringList list;
    for(size_t i = 0;i < size;i++)
    {
        list << QString::number(array[i],'g',10);
    }
    return list.join(",");
}

SWW_CORE_NAMESPACE_END

#endif // PLUGTYPEUTIL_H
