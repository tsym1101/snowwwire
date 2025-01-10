#ifndef SWW_NDTRANSFORMGEO_H
#define SWW_NDTRANSFORMGEO_H

#include "Core/Node.h"
#include "Node/PlugTypeDefFwd.h"
#include "Geom/Transform.h"

namespace SwwGeom {

class ShapePlug;

class  NdTransformGeo :public SwwGeom::Transform
{
public:
    NdTransformGeo();
    virtual ~NdTransformGeo(){}
    virtual bool initialize();
    virtual QString toolTip() const;
    virtual bool execute(const Sww::Core::PlugBase *plug);
    static Sww::Core::Node* creator();

private:
    SwwGeom::ShapePlug* _iShape;
    SwwGeom::ShapePlug* _oShape;
};

}//ns SwwGeom

#endif // SWW_NDTRANSFORMGEO_H
