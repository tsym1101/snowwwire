#ifndef SWW_LIGHT_H
#define SWW_LIGHT_H

#include "Geom/Shape.h"
#include "Geom/GL/GeomGLTypeDefFwd.h"

#include "Core/SwwAssert.h"
#include "Core/SwwMacro.h"

namespace SwwGeom {

class Light : public Shape
{

public:
    typedef std::shared_ptr<Light> Ptr;
    typedef std::shared_ptr<const Light> ConstPtr;

    enum LightType{
        kPoint,
        kDirectional,
        kSpot
    };

    Light(const std::string& name);

    virtual Shape::Ptr shallowCopy()const;

    virtual bool draw()const;
    virtual bool drawLegacy()const;
    virtual GeomType getGeomType()const{return kLight;}
    virtual LightType getLightType()const{return kPoint;}

    void clearGLShape(){_glLines.reset();}

protected:
    GLLinesPtr _glLines;
    virtual bool updateGLResources();

    SWW_SIMPLE_MEMBER(Imath::V3f,Ld)
    SWW_SIMPLE_MEMBER(Imath::V3f,Ls)
    SWW_SIMPLE_MEMBER(Imath::V3f,La)

};

}//ns

#endif // SWW_LIGHT_H
