#include "NdPointLight.h"
#include "Core/PlugType.h"
#include "Geom/PlugTypeGeom.h"
#include "Util/SwwMath.h"

namespace  SwwGeom{

NdPointLight::NdPointLight()
{
}

bool NdPointLight::initialize()
{
    SWW_CHECK_BOOL(Transform::initialize());

    hideInheritedPlug(_oTransform);

    _Ld = new Sww::Core::V3fPlug("Ld",Imath::V3f(1.0,1.0,1.0));
    _Ld->setStorable(true);
    _Ld->setConnectable(true);

    _Ls = new Sww::Core::V3fPlug("Ls",Imath::V3f(1.0,1.0,1.0));
    _Ls->setStorable(true);
    _Ls->setConnectable(true);

    _La = new Sww::Core::V3fPlug("La",Imath::V3f(0.0,0.0,0.0));
    _La->setStorable(true);
    _La->setConnectable(true);

    _light = new SwwGeom::LightPlug("light");
    _light->setOutput(true);
    _light->setConnectable(true);

    addPlug(_Ld);
    addPlug(_Ls);
    addPlug(_La);
    addPlug(_light);

    _translate->affects(_light);
    _rotate->affects(_light);
    _scale->affects(_light);
    _parentTransform->affects(_light);
    _Ld->affects(_light);
    _Ls->affects(_light);
    _La->affects(_light);

    _lightPtr.reset(new SwwGeom::Light("pointLight"));

    return true;
}
QString NdPointLight::toolTip()const
{
    QString str = "your tooltip";           
    return str;
}

Sww::Core::Node* NdPointLight::creator(){
    return new NdPointLight();
}

bool NdPointLight::execute(const Sww::Core::PlugBase *plug)
{

    if(plug == _light){

        SWW_VAL(_translate,trans);
        SWW_VAL(_rotate,rotate);
        SWW_VAL(_scale,scale);
        SWW_VAL(_parentTransform,parentTransform);

        SWW_VAL(_Ld,Ld);
        SWW_VAL(_Ls,Ls);
        SWW_VAL(_La,La);

        Imath::M44f iTransform;
        iTransform.translate(trans);
        Imath::V3d rotTmp(Sww::Util::SwwMath::degToRad(rotate.x),
                          Sww::Util::SwwMath::degToRad(rotate.y),
                          Sww::Util::SwwMath::degToRad(rotate.z));
        iTransform.rotate(rotTmp);
        iTransform.scale(scale);

        Imath::M44f oMatrix = iTransform * parentTransform;

        _lightPtr->setTransform(oMatrix);
        _lightPtr->Ld(Ld);
        _lightPtr->Ls(Ls);
        _lightPtr->La(La);

        _light->setOutputValue(_lightPtr);
    }

    return true;
}

}//ns SwwGeom

