#include "IXForm.h"
#include "OpenGL/IGLNode.h"
#include "Core/PlugType.h"
#include "Util/SwwMath.h"

namespace SwwOpenGL {
IXForm::IXForm():_translate(0),_rotate(0),
    _scale(0),_transform(0)/*,_parent(0)*/
{
}

void IXForm::attach(IGLNode *n){
    _translate = new Sww::Core::V3fPlug("translate");
    _rotate = new Sww::Core::V3fPlug("rotate");
    _scale = new Sww::Core::V3fPlug("scale",Imath::V3f(1.f,1.f,1.f));

    _translate->setConnectable(true);
    _rotate->setConnectable(true);
    _scale->setConnectable(true);

    _translate->setStorable(true);
    _rotate->setStorable(true);
    _scale->setStorable(true);

//    _parent = new Sww::Core::M44fPlug("parent");
//    _parent->setConnectable(true);
//    _parent->setHidden(true);

    _transform = new Sww::Core::M44fPlug("transform");
    _transform->setConnectable(true);
    _transform->setOutput(true);

    n->addPlug(_translate);
    n->addPlug(_rotate);
    n->addPlug(_scale);
    n->addPlug(_transform);
    //n->addPlug(_parent);

    _owner = n;
    affects(_transform);
    n->getITransformPlug()->affects(_transform);
}

void IXForm::affects(Sww::Core::PlugBase *output){
    Q_ASSERT_X(output->isOutput(),output->getName().c_str(),"must be output.");
    _translate->affects(output);
    _rotate->affects(output);
    _scale->affects(output);
    //_parent->affects(output);
}

bool IXForm::isClean()const{
    return _transform->isClean();
}

bool IXForm::applyTransform(Imath::M44f &target,
                             Imath::V3f trans, Imath::V3f rotate, Imath::V3f scale){
    target.translate(trans);
    Imath::V3d rotTmp(Sww::Util::SwwMath::degToRad(rotate.x),
                      Sww::Util::SwwMath::degToRad(rotate.y),
                      Sww::Util::SwwMath::degToRad(rotate.z));
    target.rotate(rotTmp);
    target.scale(scale);

    if(_owner->getITransformPlug()->prevCount()){
        bool ok;
        const Imath::M44f& parent = _owner->getITransformPlug()->getConstInputValue(&ok);
        if(!ok)return false;
        target = target * parent;
    }
    return true;
}

bool IXForm::getTransform(Imath::M44f& t){

    if(_transform->isClean()){
        t = _transform->rawValue();
        return true;
    }

    bool ok;
    SWW_GET(Imath::V3f,trans,_translate,ok);
    SWW_GET(Imath::V3f,rotate,_rotate,ok);
    SWW_GET(Imath::V3f,scale,_scale,ok);

    SWW_CHECK_BOOL(applyTransform(t,trans,rotate,scale));

    _transform->setOutputValue(t);
    _transform->setClean();//must for pushDirty()

    return true;
}

bool IXForm::fastGetTransform(Imath::M44f &t){
    if(_transform->isClean()){
        t = _transform->rawValue();
        return true;
    }

    SWW_CHECK_BOOL(applyTransform(t,
                                 _translate->rawValue(),
                                 _rotate->rawValue(),
                                 _scale->rawValue())
                  );

    _transform->setOutputValue(t);
    _transform->setClean();//must for pushDirty()
    return true;

}

bool IXForm::getTranslate(Imath::V3f& value){
    bool ok;
    SWW_GET(Imath::V3f,valueTmp,_translate,ok);
    value = valueTmp;
    return true;
}

bool IXForm::getRotate(Imath::V3f& value){
    bool ok;
    SWW_GET(Imath::V3f,valueTmp,_rotate,ok);
    value = valueTmp;
    return true;
}

bool IXForm::getScale(Imath::V3f& value){
    bool ok;
    SWW_GET(Imath::V3f,valueTmp,_scale,ok);
    value = valueTmp;
    return true;
}

bool IXForm::resetXForm(){

    SWW_CHECK_BOOL(_translate->setInputValue(Imath::V3f(0,0,0)));
    SWW_CHECK_BOOL(_rotate->setInputValue(Imath::V3f(0,0,0)));
    SWW_CHECK_BOOL(_scale->setInputValue(Imath::V3f(1,1,1)));

    return true;
}

}//ns
