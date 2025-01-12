#include "IShadingGroup.h"
#include "OpenGL/PlugTypeGL.h"
#include "Geom/PlugTypeGeom.h"
#include "OpenGL/GLShaderProgram.h"
#include "OpenGL/GLUniformBufferObject.h"
#include <GL/glew.h>
#include <GL/GL.h>

namespace  SwwOpenGL{

IShadingGroup::IShadingGroup()
{
}

bool IShadingGroup::initialize()
{

    _iShapePlug = new SwwGeom::ShapePlug("iShape");
    _iShapePlug->setConnectable(true);
    _iShapePlug->setMultiConnect(true);

    _shadingGroupPlug = new IShadingGroupPlug("shadingGroup");
    _shadingGroupPlug->setConnectable(true);
    _shadingGroupPlug->setOutput(true);

    addPlug(_iShapePlug);
    addPlug(_shadingGroupPlug);

    _iShapePlug->affects(_shadingGroupPlug);

    _shadingGroupPlug->setOutputValue(this);
    
    return true;
}

bool IShadingGroup::bindUbo(const std::vector<SwwOpenGL::GLUniformBufferObject *> &uboVec){
    _uboVec.clear();
    SWW_DEBUG("clear ubo.");
    for (const auto& i : uboVec) {
        Q_ASSERT(i);
        if(!_shader->bindUbo(i->getName(),i->getBindingPoint())){
            return false;
        }
        _uboVec.push_back(i);
    }
    return true;
}

bool IShadingGroup::restoreUbo()const{
    for(const auto& i : _uboVec){
        Q_ASSERT(i);
        if(!_shader->bindUbo(i->getName(),i->getBindingPoint())){
            return false;
        }
    }
    return true;
}

}//ns SwwOpenGL
