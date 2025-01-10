#include "NdDirectionalLight.h"
#include "Core/PlugType.h"
#include "Geom/DirectionalLight.h"

namespace  SwwGeom{

NdDirectionalLight::NdDirectionalLight()
{
}

bool NdDirectionalLight::initialize()
{
    SWW_CHECK_BOOL(NdPointLight::initialize());
    _lightPtr.reset(new DirectionalLight(getName()));
    return true;
}

QString NdDirectionalLight::toolTip()const
{
    QString str = "your tooltip";
    return str;
}

Sww::Core::Node* NdDirectionalLight::creator(){
    return new NdDirectionalLight();
}

bool NdDirectionalLight::execute(const Sww::Core::PlugBase *plug)
{
    SWW_CHECK_BOOL(NdPointLight::execute(plug));
    return true;
}

}//ns SwwGeom
