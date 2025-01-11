#include "PlugTypeUtil.h"

SWW_CORE_NAMESPACE_BEGIN
//PlugTypeUtil::PlugTypeUtil()
//{
//}

bool PlugTypeUtil::strToVec2i(const QString &valueStr, Imath::V2i &v)
{
    QStringList list = valueStr.split(",");
    if(list.length() != 2)return false;

    bool ok;
    v.x = list.at(0).toInt(&ok);
    if(!ok) return false;
    v.y = list.at(1).toInt(&ok);
    return ok;
}

bool PlugTypeUtil::strToVec3i(const QString &valueStr, Imath::V3i &v)
{
    QStringList list = valueStr.split(",");
    if(list.length() != 3)return false;

    bool ok;
    v.x = list.at(0).toInt(&ok);
    if(!ok) return false;
    v.y = list.at(1).toInt(&ok);
    if(!ok) return false;
    v.z = list.at(2).toInt(&ok);
    return ok;
}

bool PlugTypeUtil::strToVec2f(const QString &valueStr, Imath::V2f &v)
{
    QStringList list = valueStr.split(",");
    if(list.length() != 2)return false;

    bool ok;
    v.x = list.at(0).toFloat(&ok);
    if(!ok) return false;
    v.y = list.at(1).toFloat(&ok);

    return ok;
}

bool PlugTypeUtil::strToVec3f(const QString &valueStr, Imath::V3f &v)
{
    QStringList list = valueStr.split(",");
    if(list.length() != 3)return false;

    bool ok;
    v.x = list.at(0).toFloat(&ok);
    if(!ok) return false;
    v.y = list.at(1).toFloat(&ok);
    if(!ok) return false;
    v.z = list.at(2).toFloat(&ok);

    return ok;
}

bool PlugTypeUtil::strToVec3d(const QString &valueStr, Imath::V3d &v)
{
    QStringList list = valueStr.split(",");
    if(list.length() != 3)return false;

    bool ok;
    v.x = list.at(0).toDouble(&ok);
    if(!ok) return false;
    v.y = list.at(1).toDouble(&ok);
    if(!ok) return false;
    v.z = list.at(2).toDouble(&ok);
    return ok;
}

bool PlugTypeUtil::strToVec4f(const QString &valueStr, Imath::V4f &v)
{
    QStringList list = valueStr.split(",");
    if(list.length() != 4)return false;

    bool ok;
    v.x = list.at(0).toFloat(&ok);
    if(!ok) return false;
    v.y = list.at(1).toFloat(&ok);
    if(!ok) return false;
    v.z = list.at(2).toFloat(&ok);
    if(!ok) return false;
    v.w = list.at(3).toFloat(&ok);

    return ok;
}

bool PlugTypeUtil::strToM33f(const QString &valueStr, Imath::M33f &m)
{
    QStringList list = valueStr.split(",");
    if(list.length() != 9)return false;

    bool ok;
    for(int i = 0;i < 9;i++)
    {
        *( (float*)(&m) + i ) =  list.at(i).toFloat(&ok);
        if(!ok) return false;
    }
    return true;
}

bool PlugTypeUtil::strToM44f(const QString &valueStr, Imath::M44f &m)
{
    QStringList list = valueStr.split(",");
    if(list.length() != 16)return false;

    bool ok;
    for(int i = 0;i < 16;i++)
    {
        *( (float*)(&m) + i ) =  list.at(i).toFloat(&ok);
        if(!ok) return false;
    }
    return true;
}

bool PlugTypeUtil::strToBox3f(const QString &valueStr, Imath::Box3f &box)
{
    QStringList list = valueStr.split(",");
    if(list.length() != 6)return false;

    bool ok;
    box.min[0] = list.at(0).toFloat(&ok);
    if(!ok) return false;
    box.min[1] = list.at(1).toFloat(&ok);
    if(!ok) return false;
    box.min[2] = list.at(2).toFloat(&ok);
    if(!ok) return false;
    box.max[0] = list.at(3).toFloat(&ok);
    if(!ok) return false;
    box.max[1] = list.at(4).toFloat(&ok);
    if(!ok) return false;
    box.max[2] = list.at(5).toFloat(&ok);

    return ok;
}

SWW_CORE_NAMESPACE_END
