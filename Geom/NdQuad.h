#ifndef SWW_NDQUAD_H
#define SWW_NDQUAD_H

#include "Core/Node.h"
#include "Core/CoreAPI.h"
#include "Node/PlugTypeDefFwd.h"

#include "Geom/Transform.h"
#include "Geom/PlugTypeGeom.h"

namespace SwwGeom {

class  NdQuad :public SwwGeom::Transform , public SwwGeom::Bounds
{
public:
    NdQuad();
    virtual ~NdQuad(){}
    virtual bool initialize();
    virtual QString toolTip() const;
    virtual bool execute(const Sww::Core::PlugBase *plug);
    static Sww::Core::Node* creator();
    virtual Imath::Box3f getBounds() const;
private:
    SwwGeom::Mesh::Ptr _meshPtr;
    SwwGeom::MeshPlug* _oMesh;
    Sww::Core::V2fPlug* _min;
    Sww::Core::V2fPlug* _max;
};

}//ns OpenGL4

#endif // SWW_NDQUAD_H

