#ifndef SWW_NDINSTANCE_H
#define SWW_NDINSTANCE_H

#include "Core/Node.h"
#include "Node/PlugTypeDefFwd.h"
#include "Geom/Bounds.h"
#include "Geom/Transform.h"

namespace SwwGeom {

class MeshPlug;
class PointsPlug;
class InstancePlug;
class Instance;

class NdInstance :public SwwGeom::Transform , public SwwGeom::Bounds
{
public:
    NdInstance();
    virtual ~NdInstance(){}
    virtual bool initialize();
    virtual QString toolTip() const;
    virtual bool execute(const Sww::Core::PlugBase *plug);
    static Sww::Core::Node* creator();
    virtual Imath::Box3f getBounds() const;

private:
    SwwGeom::MeshPlug* _iMeshPlug;
    SwwGeom::PointsPlug* _iPointsPlug;

    SwwGeom::InstancePlug* _instancePlug;
    std::shared_ptr<Instance> _instance;

    Imath::Box3f _bounds;
};

}//ns SwwGeom

#endif // SWW_NDINSTANCE_H
