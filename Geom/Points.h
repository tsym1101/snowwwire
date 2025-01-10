#ifndef POINTS_H
#define POINTS_H

#include "Geom/Shape.h"
#include "Geom/GL/GeomGLTypeDefFwd.h"
#include <QFlags>

#include "Geom/ArrayProperty.h"
#include "Core/DualMap.h"

namespace SwwGeom {

class SWWGEOMSHARED_EXPORT Points : public Shape
{
public:

    typedef std::shared_ptr<Points> Ptr;
    typedef std::shared_ptr<const Points> ConstPtr;

    enum GLStateFlag{
        kNothing               = 0x00,
        kMustUpdateVerts       = 0x01,
        kMustUpdateNormals     = 0x02,
        kMustUpdateUvs         = 0x04,
        kMustUpdateColors      = 0x08
    };

    Q_DECLARE_FLAGS(GLStateFlags, GLStateFlag)

    Points(const std::string& name);

    Shape::Ptr shallowCopy()const;

    Points::Ptr deepCopy()const;

    bool valid() const;

    void setVertices(const Sww::Core::V3fArrayPtr& ptr);
    void setNormals(const Sww::Core::V3fArrayPtr& ptr);
    void setUvs(const Sww::Core::V2fArrayPtr& ptr);
    void setColors(const Sww::Core::V3fArrayPtr& ptr);
    void setAlphas(const Sww::Core::FloatArrayPtr& ptr);

    const Sww::Core::V3fArrayPtr&  getVertices()const{return _meshVertices;}
    const Sww::Core::V3fArrayPtr& getNormals()const{return _meshNormals;}
    const Sww::Core::V2fArrayPtr& getUvs()const{return _meshUvs;}
    const Sww::Core::V3fArrayPtr& getColors()const{return _meshColors;}
    const Sww::Core::FloatArrayPtr& getAlphas()const{return _meshAlphas;}

    void printVertices()const;

    bool hasNormals()const{return static_cast<bool>(_meshNormals);}
    bool hasUvs()const{return static_cast<bool>(_meshUvs);}
    bool hasColors()const{return static_cast<bool>(_meshColors);}
    bool hasAlphas()const{return static_cast<bool>(_meshAlphas);}

    virtual bool draw()const;
    virtual bool drawLegacy()const;
    virtual GeomType getGeomType()const{return kPoints;}

    const ArrayProperty::Ptr getProperty(const std::string& propName)const;//参照返ししない
    void addProperty(const SwwGeom::ArrayProperty::Ptr& prop);
    bool removeProperty(const std::string& propName);
    void clearProperty();
    std::vector<std::string> getPropertyNames()const;
    bool contains(const std::string& propName)const;

private:
    Sww::Core::V3fArrayPtr _meshVertices;
    Sww::Core::V3fArrayPtr _meshNormals;
    Sww::Core::V2fArrayPtr _meshUvs;
    Sww::Core::V3fArrayPtr _meshColors;
    Sww::Core::FloatArrayPtr _meshAlphas;

    GLPointsPtr _glPoints;

    bool updateGLResources();

    GLStateFlags _flag;

    Sww::Core::DualMap<SwwGeom::ArrayProperty::Ptr> _ArrayPropertys;

//    //このクラスはコピー禁止にしておく
//    Points(const  Points&);            // コピーコンストラクタ
//    Points operator=(const  Points&);  // 代入演算子オーバーロード
};

}//ns

#endif // POINTS_H
