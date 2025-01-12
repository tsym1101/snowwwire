#ifndef SWW_IXFORM_H
#define SWW_IXFORM_H

#include <ImathMatrix.h>
#include "OpenGL/OpenGLAPI.h"

namespace Sww::Core{
class PlugBase;
class FloatPlug;
class BoolPlug;
class IntPlug;
class V3fPlug;
class V4fPlug;
class M44fPlug;
class VoidPlug;
}

namespace SwwOpenGL {
class IGLNode;

class SWWOPENGLSHARED_EXPORT IXForm
{
public:
    IXForm();
    void attach(IGLNode* n);
    bool valid(){return _translate && _rotate && _scale &&
                _transform /*&& _parent*/;}

    bool getTransform(Imath::M44f &t);
    bool fastGetTransform(Imath::M44f &t);//評価なし　TRSにはコネクトしない前提

    bool getTranslate(Imath::V3f& value);
    bool getRotate(Imath::V3f& value);
    bool getScale(Imath::V3f& value);

    Sww::Core::V3fPlug* translatePlug()const {return _translate;}
    Sww::Core::V3fPlug* rotatePlug()const {return _rotate;}
    Sww::Core::V3fPlug* scalePlug()const {return _scale;}
    Sww::Core::M44fPlug* transformPlug()const {return _transform;}
    //Sww::Core::M44fPlug* parentPlug()const {return _parent;}

    void affects(Sww::Core::PlugBase* output);

    bool isClean()const;

    bool resetXForm();
private:
    Sww::Core::V3fPlug* _translate;
    Sww::Core::V3fPlug* _rotate;
    Sww::Core::V3fPlug* _scale;
    Sww::Core::M44fPlug* _transform;
    //Sww::Core::M44fPlug* _parent;
    IGLNode* _owner;

    bool applyTransform(Imath::M44f& target,
                        Imath::V3f trans,Imath::V3f rotate,Imath::V3f scale);
    IXForm(const IXForm& p);  //コピーコンストラクタ
    IXForm &operator=(const IXForm& p);   //代入演算子オーバーロード
};

}//ns
#endif // SWW_IXFORM_H
