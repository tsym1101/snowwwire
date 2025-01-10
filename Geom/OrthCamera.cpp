#include "OrthCamera.h"
#include "Core/PlugType.h"
#include <glm/gtc/matrix_transform.hpp>
#include <PlugTypeGeom.h>

namespace  SwwGeom{

OrthCamera::OrthCamera()
{
}

bool OrthCamera::initialize()
{
    SWW_CHECK_BOOL(Camera::initialize());

    _viewVolumeSize = new Sww::Core::V2fPlug("viewVolumeSize",Imath::V2f(2.f,2.f));
    _viewVolumeSize->setConnectable(true);
    _viewVolumeSize->setStorable(true);

    //    _lockMouseOperation = new Sww::Core::BoolPlug("lockMouseOperation",true);
    //    _lockMouseOperation->setStorable(true);
    _lockMouseOperation->setInputValue(true);

    addPlug(_viewVolumeSize);
    //addPlug(_lockMouseOperation);

    _viewVolumeSize->affects(_camera);
    _viewVolumeSize->affects(_oShape);
    _nearClipPlug->affects(_oShape);
    _farClipPlug->affects(_oShape);

    return true;
}
QString OrthCamera::toolTip()const
{
    QString str = "your tooltip";
    return str;
}

Sww::Core::Node* OrthCamera::creator(){
    return new OrthCamera();
}

bool OrthCamera::updateShape(){
    Sww::Core::V3fArrayPtr linesVertices(new Sww::Core::V3fArray());
    Sww::Core::IntArrayPtr counts(new Sww::Core::IntArray());

    SWW_VAL(_viewVolumeSize,viewVolumeSize);
    SWW_VAL(_nearClipPlug,nearClip);
    SWW_VAL(_farClipPlug,farClip);

    float px = viewVolumeSize.x / 2.0;
    float py = viewVolumeSize.y / 2.0;
    float pzNear = -nearClip;
    float pzFar = -farClip;

    linesVertices->push_back(Imath::V3f(-px,py,pzNear));
    linesVertices->push_back(Imath::V3f(-px,-py,pzNear));
    linesVertices->push_back(Imath::V3f(px,-py,pzNear));
    linesVertices->push_back(Imath::V3f(px,py,pzNear));
    linesVertices->push_back(Imath::V3f(-px,py,pzNear));
    linesVertices->push_back(Imath::V3f(-px,py,pzFar));
    linesVertices->push_back(Imath::V3f(-px,-py,pzFar));
    linesVertices->push_back(Imath::V3f(px,-py,pzFar));
    linesVertices->push_back(Imath::V3f(px,py,pzFar));
    linesVertices->push_back(Imath::V3f(-px,py,pzFar));

    linesVertices->push_back(Imath::V3f(-px,-py,pzNear));
    linesVertices->push_back(Imath::V3f(-px,-py,pzFar));

    linesVertices->push_back(Imath::V3f(px,-py,pzNear));
    linesVertices->push_back(Imath::V3f(px,-py,pzFar));

    linesVertices->push_back(Imath::V3f(px,py,pzNear));
    linesVertices->push_back(Imath::V3f(px,py,pzFar));

    counts->push_back(10);
    counts->push_back(2);
    counts->push_back(2);
    counts->push_back(2);

    linesVertices->push_back(Imath::V3f(-0.5,0.0,0.0));
    linesVertices->push_back(Imath::V3f(0.5,0.0,0.0));
    linesVertices->push_back(Imath::V3f(0.0,0.0,-0.5));
    linesVertices->push_back(Imath::V3f(0.0,0.0,0.5));
    linesVertices->push_back(Imath::V3f(0.0,-0.5,0.0));
    linesVertices->push_back(Imath::V3f(0.0,0.5,0.0));

    counts->push_back(2);
    counts->push_back(2);
    counts->push_back(2);

    _shapePtr->setVertices(linesVertices);
    _shapePtr->setCounts(counts);

    return true;
}

bool OrthCamera::execute(const Sww::Core::PlugBase *plug)
{
    //SWW_CHECK_BOOL(Camera::execute(plug));
    //SWW_JUST_UPDATE(_viewVolumeSize);

    if(plug == _oShape){

        if(!_shapePtr){
            _shapePtr.reset(new SwwGeom::Lines(getName()+ "_cameraShape"));
            SWW_CHECK_BOOL(updateShape());
            _oShape->setOutputValue(_shapePtr);
        }

        Imath::M44f transform;
        SWW_CHECK_BOOL(getTransform(transform));
        _shapePtr->setTransform(transform);

        if(!_viewVolumeSize->isClean() ||
                !_nearClipPlug->isClean() ||
                !_farClipPlug->isClean()){
            SWW_CHECK_BOOL(updateShape());
        }
    }
    else if(plug == _oTransform){
        Imath::M44f m;
        SWW_CHECK_BOOL(getTransform(m));
    }

    return true;
}

Imath::M44f OrthCamera::makeViewMatrix()const
{
    return Camera::makeViewMatrix();
}

Imath::M44f OrthCamera::makeProjectionMatrix(float deviceAspect)const //引数使わない
{
    bool ok;
    const Imath::V2f& orthoViewVolumeSize = _viewVolumeSize->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("makeProjectionMatrix failed. " << getName() );
        return Imath::M44f();
    }
    float nearClip = _nearClipPlug->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("makeProjectionMatrix failed. " << getName() );
        return Imath::M44f();
    }
    float   farClip = _farClipPlug->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("makeProjectionMatrix failed. " << getName() );
        return Imath::M44f();
    }

    //視点座標変換************************************************************************
    glm::mat4 projTmp = glm::ortho((float)(-orthoViewVolumeSize.x/2.0),
                                   (float)(orthoViewVolumeSize.x/2.0),
                                   (float)(-orthoViewVolumeSize.y/2.0),
                                   (float)(orthoViewVolumeSize.y/2.0),
                                   (float)(nearClip),
                                   (float)(farClip));

    Imath::M44f projectionMatrix = *((Imath::M44f*)&projTmp);

    return projectionMatrix;

}


void OrthCamera::operate(int dx, int dy,int viewportWidth, int viewportHeight,const QMouseEvent *event)
{
    SWW_VAL_VOID(_lockMouseOperation,lockMouse);
    if(!lockMouse)Camera::operate(dx,dy,viewportWidth,viewportHeight,event);
}


}//ns SwwGeom

