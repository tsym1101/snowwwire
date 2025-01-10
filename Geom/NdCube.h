#ifndef SWW_NDCUBE_H
#define SWW_NDCUBE_H

#include "Core/Node.h"
#include "Node/PlugTypeDefFwd.h"
#include "Geom/Transform.h"
#include "Geom/PlugTypeGeom.h"

namespace SwwGeom {

class  NdCube :public SwwGeom::Transform , public SwwGeom::Bounds
{
public:
    NdCube();
    virtual ~NdCube(){}
    virtual bool initialize();
    virtual QString toolTip() const;
    virtual bool execute(const Sww::Core::PlugBase *plug);
    static Sww::Core::Node* creator();
    virtual Imath::Box3f getBounds() const;
private:
    SwwGeom::Mesh::Ptr _meshPtr;
    SwwGeom::MeshPlug* _oMesh;
};

}//ns SwwGeom

#endif // SWW_NDCUBE_H

