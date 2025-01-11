#include "NdSpotLight.h"
#include "Geom/PlugTypeGeom.h"
#include "Geom/SpotLight.h"
#include "Util/SwwMath.h"

namespace  SwwGeom{

NdSpotLight::NdSpotLight():_mustUpdateShape(true)
{
}

bool NdSpotLight::initialize()
{
    SWW_CHECK_BOOL(NdPointLight::initialize());

    float defaultAngle = 45;
    _angle = new Sww::Core::FloatPlug("angle",defaultAngle);
    _angle->setStorable(true);
    _angle->setConnectable(true);

    addPlug(_angle);

    _angle->affects(_light);

    _spotLightPtr = new SpotLight(getName());
    _lightPtr.reset(_spotLightPtr);
    _spotLightPtr->angle(defaultAngle);

    return true;
}

QString NdSpotLight::toolTip()const
{
    QString str = "your tooltip";
    return str;
}

Sww::Core::Node* NdSpotLight::creator(){
    return new NdSpotLight();
}


bool NdSpotLight::postSetProcess(const Sww::Core::PlugBase *p){
    if(p == _angle){
      _mustUpdateShape = true;
    }
    return true;
}

bool NdSpotLight::execute(const Sww::Core::PlugBase *plug)
{
    if(plug == _light){

        SWW_VAL(_translate,trans);
        SWW_VAL(_rotate,rotate);
        SWW_VAL(_scale,scale);
        SWW_VAL(_parentTransform,parentTransform);

        SWW_VAL(_Ld,Ld);
        SWW_VAL(_Ls,Ls);
        SWW_VAL(_La,La);

        SWW_VAL(_angle,angle);

        Imath::M44f iTransform;
        iTransform.translate(trans);
        Imath::V3d rotTmp(Sww::Util::SwwMath::degToRad(rotate.x),
                          Sww::Util::SwwMath::degToRad(rotate.y),
                          Sww::Util::SwwMath::degToRad(rotate.z));
        iTransform.rotate(rotTmp);
        iTransform.scale(scale);

        Imath::M44f oMatrix = iTransform * parentTransform;

        _spotLightPtr->setTransform(oMatrix);
        _spotLightPtr->Ld(Ld);
        _spotLightPtr->Ls(Ls);
        _spotLightPtr->La(La);
        _spotLightPtr->angle(angle);

        _light->setOutputValue(_lightPtr);

        if(_mustUpdateShape){
            _spotLightPtr->clearGLShape();
            _mustUpdateShape = false;
        }
    }

    return true;
}

}//ns SwwGeom
