#ifndef SWW_SHAPE_H
#define SWW_SHAPE_H

#include <ImathMatrix.h>
#include <ImathBox.h>

#include "Core/SwwTypeDef.h"
#include <memory>

#include "Geom/GeomAPI.h"
#include "Geom/Bounds.h"

#include <ImathMatrixAlgo.h>
#include <ImathMatrix.h>
#include <ImathVec.h>
#include <ImathVecAlgo.h>

namespace SwwGeom {

class SWWGEOMSHARED_EXPORT Shape : public Bounds
{
public:
    typedef std::shared_ptr<Shape> Ptr;
    typedef std::shared_ptr<const Shape> ConstPtr;

    enum GeomType{
        kPoints,
        kLines,
        kMesh,
        kLight,
        kInstance,
        kUnknown
    };

    Shape(const std::string& name);
    virtual GeomType getGeomType()const = 0;
    virtual Ptr shallowCopy()const = 0;

    const std::string& getName()const;
    void setName(const std::string& name);

    void setTransform(const Imath::M44f& m);
    void applyTransform(const Imath::M44f& m);

    const Imath::M44f& getTransform()const;
    Imath::M44d getTransformd()const;
    Imath::V3f getTranslation()const;
    Imath::V3f getDirection()const;

    virtual bool draw()const{return false;}
    virtual bool drawLegacy()const{return false;}

    void setDynamic(bool b){_isDynamic = b;}
    bool isDynamic()const{return _isDynamic;}

    void setBounds(const Imath::Box3f &b){_bbox = b;}
    void computeBounds(const Sww::Core::V3fArrayConstPtr &posArray);

    virtual Imath::Box3f getBounds()const;
    Imath::Box3f getNonInheritedBounds()const{return _bbox;}

protected:
    std::string _name;
    Imath::M44f _iTransform;
    bool _isDynamic;

    Imath::Box3f _bbox;//transformされてない生のbbox
};

inline const std::string& Shape::getName()const{
    return _name;
}

inline  void Shape::setName(const std::string &name){
    _name = name;
}

inline void Shape::setTransform(const Imath::M44f &m){
    _iTransform = m;
}

inline void Shape::applyTransform(const Imath::M44f &m){
    _iTransform =  _iTransform * m;
}

inline const Imath::M44f& Shape::getTransform() const{
    return _iTransform;
}

inline Imath::M44d Shape::getTransformd() const{
    Imath::M44d md;

    md[0][0] = static_cast<double>(_iTransform[0][0]);
    md[0][1] = static_cast<double>(_iTransform[0][1]);
    md[0][2] = static_cast<double>(_iTransform[0][2]);
    md[0][3] = static_cast<double>(_iTransform[0][3]);
    md[1][0] = static_cast<double>(_iTransform[1][0]);
    md[1][1] = static_cast<double>(_iTransform[1][1]);
    md[1][2] = static_cast<double>(_iTransform[1][2]);
    md[1][3] = static_cast<double>(_iTransform[1][3]);
    md[2][0] = static_cast<double>(_iTransform[2][0]);
    md[2][1] = static_cast<double>(_iTransform[2][1]);
    md[2][2] = static_cast<double>(_iTransform[2][2]);
    md[2][3] = static_cast<double>(_iTransform[2][3]);
    md[3][0] = static_cast<double>(_iTransform[3][0]);
    md[3][1] = static_cast<double>(_iTransform[3][1]);
    md[3][2] = static_cast<double>(_iTransform[3][2]);
    md[3][3] = static_cast<double>(_iTransform[3][3]);

    return md;
}

inline Imath::V3f Shape::getTranslation()const{
    return _iTransform.translation();
}


}//ns

#endif // SWW_SHAPE_H
