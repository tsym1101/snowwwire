#ifndef SWW_GLOBJECTI_H
#define SWW_GLOBJECTI_H
#include "OpenGL/IGLNode.h"
#include "OpenGL/RenderDevice.h"
#include "OpenGL/IXForm.h"
#include <ImathMatrix.h>

namespace SwwOpenGL{
class IGLNodePlug;

class  SWWOPENGLSHARED_EXPORT IGLObject :public IGLNode
{
public:
    IGLObject();
    virtual ~IGLObject(){}
    virtual bool initialize();
    virtual bool executeGL(const Sww::Core::PlugBase *plug,const SwwOpenGL::RenderDevice& device);

    virtual Imath::Box3f getBBox();
    virtual void setBbox(const Imath::Box3f& b){_bbox = b;}

    bool getTranslate(Imath::V3f& value){
        Imath::M44f m;
        SWW_CHECK_BOOL(_xform.getTransform(m));
        value = m.translation();
        return true;
    }
    bool getTransform(Imath::M44f& value){
        return _xform.getTransform(value);
    }

    bool resetXForm(){return _xform.resetXForm();}

protected:
    void hideTransformPlug();
    IXForm _xform;
    SwwOpenGL::IGLNodePlug* _drawObject;
};
}//ns SwwOpenGL
#endif // SWW_GLOBJECTI_H
