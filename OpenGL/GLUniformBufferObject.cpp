#include "GLUniformBufferObject.h"

#include "OpenGL/SwwGLMacro.h"
namespace SwwOpenGL {

//GLuint GLUniformBufferObject::_currentUbo = 0;

QMap<int,GLUniformBufferObject*> GLUniformBufferObject::_bindingPointTable;

GLUniformBufferObject::GLUniformBufferObject(const std::string &name,
                                             GLuint dataSize):
    _name(name),_dataSize(dataSize),_bindingPoint(0)
{
    SWW_CHECK_GL_CONTEXT;

    getAvailableBindingPoint();
    if(!_bindingPoint)return;

    glGenBuffers(1,&_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER,_ubo);
    glBufferData(GL_UNIFORM_BUFFER,_dataSize,0,GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER,0);

    SWW_DEBUG(getName() << ":" << _bindingPoint);
}

bool GLUniformBufferObject::valid()const{
//    SWW_DEBUG_VALUE(_bindingPoint);
//    SWW_DEBUG_VALUE(static_cast<bool>(_bindingPoint));
    return static_cast<bool>(_bindingPoint);
}

GLUniformBufferObject::~GLUniformBufferObject(){
    SWW_CHECK_GL_CONTEXT;

    //    if(!QGLContext::currentContext()){
    //        SWW_ERR("no context is current.");
    //    }
    _bindingPointTable.remove(_bindingPoint);

    glDeleteBuffers(1,&_ubo);
}

void GLUniformBufferObject::getAvailableBindingPoint(){
    GLint maxUniformBufferBinding;
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxUniformBufferBinding);
    //SWW_DEBUG_VALUE(maxUniformBUfferBinding);

    if(_bindingPointTable.size() >= maxUniformBufferBinding){
        SWW_ERR("uniform buffer binding is NOT available. maxUniformBufferBinding : " << maxUniformBufferBinding);
        return;
    }

    for(int i = 1;i < maxUniformBufferBinding;i++){
        if(!_bindingPointTable.contains(i)){
            _bindingPointTable[i] = this;
            _bindingPoint = i;
            break;
        }
    }

}


}//ns
