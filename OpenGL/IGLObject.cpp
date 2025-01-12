#include "IGLObject.h"
#include "OpenGL/PlugTypeGL.h"

#include <GL/glew.h>
#include <GL/GL.h>

#include <ImathBoxAlgo.h>

namespace  SwwOpenGL{

IGLObject::IGLObject()
{
}

bool IGLObject::initialize()
{
    SWW_CHECK_BOOL(IGLNode::initialize());
    _xform.attach(this);

    _drawObject = new SwwOpenGL::IGLNodePlug("drawObject",this);
    _drawObject->setOutput(true);
    _drawObject->setConnectable(true);

    addPlug(_drawObject);

    _xform.affects(_drawObject);

    setBbox(Imath::Box3f(Imath::V3f(-1.0,-1.0,-1.0),Imath::V3f(1.0,1.0,1.0)));

    return true;
}

bool IGLObject::executeGL(const Sww::Core::PlugBase *plug,const SwwOpenGL::RenderDevice& device)
{
    if(plug == _drawObject){
        
    }
    return true;
}

Imath::Box3f IGLObject::getBBox(){
    Imath::M44f mat;
    if(!_xform.getTransform(mat)){
        SWW_ERR("makeModelViewMatrix failed.");
        return _bbox;
    }
    Imath::Box3f result = Imath::transform(_bbox,mat);
    return  result;
}

void IGLObject::hideTransformPlug()
{
    hideInheritedPlug(_xform.translatePlug());
    hideInheritedPlug(_xform.rotatePlug());
    hideInheritedPlug(_xform.scalePlug());
    hideInheritedPlug(_xform.transformPlug());
    //hideInheritedPlug(_xform.parentPlug());
}

}//ns SwwOpenGL
