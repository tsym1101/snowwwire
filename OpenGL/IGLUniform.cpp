#include "IGLUniform.h"
#include "OpenGL/GLShaderProgram.h"
#include "OpenGL/SwwGLMacro.h"

namespace SwwOpenGL {

IGLUniform::IGLUniform(const SwwOpenGL::GLShaderProgramPtr& shader,const std::string& name)
{
    init(shader,name);
}

bool IGLUniform::checkShaderActive()const{

    GLint id;
    glGetIntegerv(GL_CURRENT_PROGRAM,(GLint*)&id);
    if(id == _programId)return true;
    else{
        SWW_ERR("invalid shader programId." << " id:" << id << " _programId:" << _programId);
        return false;
    }

    return true;
}

void IGLUniform::init(const SwwOpenGL::GLShaderProgramPtr &shader, const std::string &name)
{
    SWW_CHECK_GL_CONTEXT;
    Q_ASSERT_X(shader,"GLUniformPlug init.",name.c_str());

    if(!shader->hasUniform(name)){
        SWW_WARN("active uniform variable not found. " << name.c_str());
    }

    shader->bind();
    _programId = shader->programId();
    _location = glGetUniformLocation(_programId,name.c_str());
    shader->release();

    _name = name;
}






}// ns

