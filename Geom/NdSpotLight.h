#ifndef SWW_NdSpotLight_H
#define SWW_NdSpotLight_H

#include "Core/Node.h"
#include "Node/PlugTypeDefFwd.h"
#include "Geom/NdPointLight.h"
#include "Geom/SpotLight.h"

namespace SwwGeom {

class  NdSpotLight :public NdPointLight
{
public:
    NdSpotLight();
    virtual ~NdSpotLight(){}
    virtual bool initialize();
    virtual QString toolTip() const;
    virtual bool postSetProcess(const Sww::Core::PlugBase *p);
    virtual bool execute(const Sww::Core::PlugBase *plug);
    static Sww::Core::Node* creator();

private:
    Sww::Core::FloatPlug* _angle;
    bool _mustUpdateShape;
   SpotLight* _spotLightPtr;
};

}//ns SwwGeom

#endif // SWW_NdSpotLight_H
