#include "Transform.h"
#include "Core/PlugType.h"
#include "Util/SwwMath.h"

namespace  SwwGeom{

Transform::Transform()
{
}

bool Transform::initialize()
{
    _parentTransform = new Sww::Core::M44fPlug("parentTransform");
    _parentTransform->setConnectable(true);
    _parentTransform->setStorable(true);

    _translate = new Sww::Core::V3fPlug("translate");
    _rotate = new Sww::Core::V3fPlug("rotate");
    _scale = new Sww::Core::V3fPlug("scale",Imath::V3f(1.f,1.f,1.f));

    _translate->setConnectable(true);
    _rotate->setConnectable(true);
    _scale->setConnectable(true);

    _translate->setStorable(true);
    _rotate->setStorable(true);
    _scale->setStorable(true);

    _oTransform = new Sww::Core::M44fPlug("oTransform");
    _oTransform->setConnectable(true);
    _oTransform->setOutput(true);

    addPlug(_parentTransform);
    addPlug(_translate);
    addPlug(_rotate);
    addPlug(_scale);
    addPlug(_oTransform);

    _parentTransform->affects(_oTransform);
    _translate->affects(_oTransform);
    _rotate->affects(_oTransform);
    _scale->affects(_oTransform);

    return true;
}
QString Transform::toolTip()const
{
    QString str = "your tooltip";           
    return str;
}

Sww::Core::Node* Transform::creator(){
    return new Transform();
}

bool Transform::getTransform(Imath::M44f &t){

    if(_oTransform->isClean()){
        _oTransform->getOutputValue(t);
        //t = _oTransform->rawValue();
        return true;
    }

    SWW_VAL(_translate,trans);
    SWW_VAL(_rotate,rotate);
    SWW_VAL(_scale,scale);
    SWW_VAL(_parentTransform,parentTransform);

    Imath::M44f iTransform;
    iTransform.translate(trans);
    Imath::V3d rotTmp(Sww::Util::SwwMath::degToRad(rotate.x),
                      Sww::Util::SwwMath::degToRad(rotate.y),
                      Sww::Util::SwwMath::degToRad(rotate.z));
    iTransform.rotate(rotTmp);
    iTransform.scale(scale);

    t = iTransform * parentTransform;
    _oTransform->setOutputValue(t);
    _oTransform->setClean();//must for pushDirty()

    return true;
}

bool Transform::execute(const Sww::Core::PlugBase *plug)
{
    if(plug == _oTransform){

        SWW_VAL(_translate,trans);
        SWW_VAL(_rotate,rotate);
        SWW_VAL(_scale,scale);
        SWW_VAL(_parentTransform,parentTransform);

        Imath::M44f iTransform;
        iTransform.translate(trans);
        Imath::V3d rotTmp(Sww::Util::SwwMath::degToRad(rotate.x),
                          Sww::Util::SwwMath::degToRad(rotate.y),
                          Sww::Util::SwwMath::degToRad(rotate.z));
        iTransform.rotate(rotTmp);
        iTransform.scale(scale);

        Imath::M44f oMatrix = iTransform * parentTransform;
        _oTransform->setOutputValue(oMatrix);
    }

    return true;
}

}//ns SwwGeom

