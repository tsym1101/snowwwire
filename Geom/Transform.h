#ifndef SWW_NDTRANSFORM_H
#define SWW_NDTRANSFORM_H

#include "Core/Node.h"
#include "Core/PlugTypeDefFwd.h"
#include <ImathMatrix.h>
#include "Geom/GeomAPI.h"

namespace SwwGeom {

class  SWWGEOMSHARED_EXPORT Transform :public Sww::Core::Node
{
public:
    Transform();
    virtual ~Transform(){}
    virtual bool initialize();
    virtual QString toolTip() const;
    virtual bool execute(const Sww::Core::PlugBase *plug);
    static Sww::Core::Node* creator();

    bool getTransform(Imath::M44f& t);

protected:
    Sww::Core::V3fPlug* _translate;
    Sww::Core::V3fPlug* _rotate;
    Sww::Core::V3fPlug* _scale;
    Sww::Core::M44fPlug* _parentTransform;
    Sww::Core::M44fPlug* _oTransform;

};

}//ns SwwGeom

#endif // SWW_NDTRANSFORM_H

