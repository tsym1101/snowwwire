#ifndef SWW_NDPOINTLIGHT_H
#define SWW_NDPOINTLIGHT_H

#include "Geom/Transform.h"
#include "Node/PlugTypeDefFwd.h"
#include "Geom/Light.h"

namespace SwwGeom {

class LightPlug;

class  NdPointLight : public SwwGeom::Transform
{
public:
    NdPointLight();
    virtual ~NdPointLight(){}
    virtual bool initialize();
    virtual QString toolTip() const;
    virtual bool execute(const Sww::Core::PlugBase *plug);
    static Sww::Core::Node* creator();


protected:
   Sww::Core::V3fPlug* _Ld;
   Sww::Core::V3fPlug* _Ls;
   Sww::Core::V3fPlug* _La;

   SwwGeom::LightPlug* _light;
   SwwGeom::Light::Ptr _lightPtr;

};

}//ns SwwGeom

#endif // SWW_NDPOINTLIGHT_H

