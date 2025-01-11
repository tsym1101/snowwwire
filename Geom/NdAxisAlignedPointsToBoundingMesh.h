#ifndef SWW_NDAXISALIGNEDPOINTSTOBOUNDINGMESH_H
#define SWW_NDAXISALIGNEDPOINTSTOBOUNDINGMESH_H

#include "Core/Node.h"
#include "Node/PlugTypeDefFwd.h"

namespace SwwGeom {

class PointsPlug;
class MeshPlug;

class  NdAxisAlignedPointsToBoundingMesh :public Sww::Core::Node
{
public:
    NdAxisAlignedPointsToBoundingMesh();
    virtual ~NdAxisAlignedPointsToBoundingMesh(){}
    virtual bool initialize();
    virtual QString toolTip() const;
    virtual bool execute(const Sww::Core::PlugBase *plug);
    static Sww::Core::Node* creator();

private:
    PointsPlug* _iPoints;
    Sww::Core::V3iPlug* _dims;
    MeshPlug* _oMesh;
    Sww::Core::BoolPlug* _offsetHalfPixel;
    Sww::Core::BoolPlug* _asLevelPlane;
};

}//ns SwwGeom

#endif // SWW_NDAXISALIGNEDPOINTSTOBOUNDINGMESH_H
