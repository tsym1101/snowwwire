#include "NdExtractVP.h"
#include "Geom/PlugTypeGeom.h"
#include "Geom/Camera.h"

namespace  SwwGeom{

NdExtractVP::NdExtractVP()
{
}

bool NdExtractVP::initialize()
{

    _camera = new SwwGeom::CameraPlug("cam");
    _camera->setConnectable(true);

    _deviceAspect = new Sww::Core::V2fPlug("deviceAspect",Imath::V2f(1.0,1.0));
    _deviceAspect->setConnectable(true);
    _deviceAspect->setStorable(true);

    _V = new Sww::Core::M44fPlug("V");
    _V->setConnectable(true);
    _V->setOutput(true);

    _P = new Sww::Core::M44fPlug("P");
    _P->setConnectable(true);
    _P->setOutput(true);

    _VP = new Sww::Core::M44fPlug("VP");
    _VP->setConnectable(true);
    _VP->setOutput(true);

    addPlug(_camera);
    addPlug(_deviceAspect);
    addPlug(_V);
    addPlug(_P);
    addPlug(_VP);

    _camera->affects(_V);
    _camera->affects(_P);
    _camera->affects(_VP);

    _deviceAspect->affects(_P);
    _deviceAspect->affects(_VP);

    return true;
}
QString NdExtractVP::toolTip()const
{
    QString str = "your tooltip";
    return str;
}

Sww::Core::Node* NdExtractVP::creator(){
    return new NdExtractVP();
}

bool NdExtractVP::getV(Imath::M44f &m){
    SWW_VAL(_camera,cam);
    m = cam->makeViewMatrix();
    _V->setOutputValue(m);
    _V->setClean(); // for push dirty.
    return true;
}

bool NdExtractVP::getP(Imath::M44f& m){
    SWW_VAL(_camera,cam);
    SWW_VAL(_deviceAspect,deviceAspect);
    float aspect = deviceAspect.x / deviceAspect.y;
    m = cam->makeProjectionMatrix(aspect);
    _P->setOutputValue(m);
    _P->setClean();// for push dirty.
    return true;
}

bool NdExtractVP::execute(const Sww::Core::PlugBase *plug)
{
    if(plug == _V){
        SWW_VAL(_camera,cam);
        Imath::M44f m = cam->makeViewMatrix();
        _V->setOutputValue(m);
    }
    else if(plug == _P){
        SWW_VAL(_camera,cam);
        SWW_VAL(_deviceAspect,deviceAspect);
        float aspect = deviceAspect.x / deviceAspect.y;
        Imath::M44f m = cam->makeProjectionMatrix(aspect);
        _P->setOutputValue(m);
    }
    else if(plug == _VP){
        Imath::M44f V,P,VP;
        if(_V->isClean()){
            V = _V->rawValue();
        }else{
           SWW_CHECK_BOOL(getV(V));
        }

        if(_P->isClean()){
            P = _P->rawValue();
        }else{
           SWW_CHECK_BOOL(getP(P));
        }

        VP = V * P;
        _VP->setOutputValue(VP);
    }

    return true;
}

}//ns SwwGeom
