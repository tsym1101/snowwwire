#ifndef SWW_INSTANCE_H
#define SWW_INSTANCE_H

#include "Geom/Shape.h"
#include "Geom/GL/GeomGLTypeDefFwd.h"

namespace SwwGeom {

class Mesh;

class SWWGEOMSHARED_EXPORT Instance : public Shape
{
public:
    typedef std::shared_ptr<Instance> Ptr;
    typedef std::shared_ptr<const Instance> ConstPtr;

    Instance(const std::string& name);
    virtual GeomType getGeomType()const{return kInstance;}

    virtual bool draw()const;
    virtual bool drawLegacy()const;

    virtual Shape::Ptr shallowCopy()const;

    void setMatArray(const Sww::Core::M44fArrayPtr& matArray);
    void setMagArray(const Sww::Core::FloatArrayPtr& magArray);

//    void setMatArray(Imath::M44f* matArray,size_t size);
//    void setMagArray(float* magArray,size_t size);

    void setBaseMesh(const Mesh* baseMesh);

private:
    Sww::Core::M44fArrayPtr _matArray;//インスタンスごとの姿勢行列
    Sww::Core::FloatArrayPtr _magArray;//インスタンスごとのスカラー値

//    Imath::M44f* _matArray;
//    float* _magArray;

    const SwwGeom::Mesh* _baseMesh;
    SwwGeom::GLInstancePtr _glInstance;
    bool updateGLResources();
    bool _mustUpdateInstance;
    bool _mustUpdateMagnitude;
    bool _mustUpdateMesh;
};

}//ns

#endif // SWW_INSTANCE_H
