#include "UniformPlugFactory.h"
#include "OpenGL/IGLUniform.h"
#include "Core/PlugType.h"
#include "OpenGL/PlugTypeGL.h"

namespace SwwOpenGL{

UniformPlugFactory::UniformPlugFactory():_texUnitCounter(1) //カウントアップ１からじゃないとだめなケースがあった
{

}

Sww::Core::PlugBase* UniformPlugFactory::generate(const SwwOpenGL::GLShaderProgramPtr& shader, const std::string& name,GLenum type)

{
    Sww::Core::PlugBase* plug = 0;
    switch (type) {
    case GL_BOOL:
        plug = new UnBoolPlug(shader,name);
        break;
    case GL_FLOAT:
        plug = new UnFloatPlug(shader,name);
        break;
    case GL_INT:
        plug = new UnIntPlug(shader,name);
        break;
    case GL_FLOAT_VEC2:
        plug = new UnV2fPlug(shader,name);
        break;
    case GL_FLOAT_VEC3:
        SWW_DEBUG_VALUE(name);
        plug = new UnV3fPlug(shader,name);
        break;
    case GL_FLOAT_VEC4:
        plug = new UnV4fPlug(shader,name);
        break;
    case GL_FLOAT_MAT3:
        plug = new UnM33fPlug(shader,name);
        break;
    case GL_FLOAT_MAT4:
        plug = new UnM44fPlug(shader,name);
        break;
    case GL_INT_VEC2:
        plug = new UnV2iPlug(shader,name);
        break;
    case GL_SAMPLER_1D :
        plug = new UnTexturePlug(shader,_texUnitCounter,name,SwwOpenGL::GLTexturePtr(),GL_TEXTURE_1D);
        _texUnitCounter++;
        break;
    case GL_SAMPLER_2D :
        plug = new UnTexturePlug(shader,_texUnitCounter,name,SwwOpenGL::GLTexturePtr(),GL_TEXTURE_2D);
        _texUnitCounter++;
        break;
    case GL_SAMPLER_2D_ARRAY :
        plug = new UnTexturePlug(shader,_texUnitCounter,name,SwwOpenGL::GLTexturePtr(),GL_TEXTURE_2D_ARRAY);
        _texUnitCounter++;
        break;
    case GL_SAMPLER_2D_RECT :
        plug = new UnTexturePlug(shader,_texUnitCounter,name,SwwOpenGL::GLTexturePtr(),GL_TEXTURE_RECTANGLE);
        _texUnitCounter++;
        break;
    case GL_SAMPLER_3D :
        plug = new UnTexturePlug(shader,_texUnitCounter,name,SwwOpenGL::GLTexturePtr(),GL_TEXTURE_3D);
        _texUnitCounter++;
        break;
    case GL_SAMPLER_CUBE :
        plug = new UnTexturePlug(shader,_texUnitCounter,name,SwwOpenGL::GLTexturePtr(),GL_TEXTURE_CUBE_MAP);
        _texUnitCounter++;
        break;
    case GL_SAMPLER_2D_SHADOW :
        plug = new UnTexturePlug(shader,_texUnitCounter,name,SwwOpenGL::GLTexturePtr(),GL_SAMPLER_2D_SHADOW);
        _texUnitCounter++;
        break;
    case GL_SAMPLER_CUBE_SHADOW :
        plug = new UnTexturePlug(shader,_texUnitCounter,name,SwwOpenGL::GLTexturePtr(),GL_SAMPLER_CUBE_SHADOW);
        _texUnitCounter++;
        break;

    default:
        SWW_ERR("unsuppurted uniform type." << name << " : 0x" << std::hex << std::uppercase << type << std::dec);
        Q_ASSERT_X(false,name.c_str(),"unsuppurted uniform type.");
        plug = new Sww::Core::NullPlug(name);
        //return false;
        break;
    }

    return plug;
}

/////////////////////////////////////////////////////////////////

#define SWW_UNPLUG_IMPL(plugName,glUploadFunc)\
    Un##plugName::Un##plugName(const SwwOpenGL::GLShaderProgramPtr& shader,const std::string& name,plugName::thisType data)\
    :Sww::Core::plugName(name,data),IGLUniform(shader,name){}\
    bool Un##plugName::justUpdate(){\
    bool stat;\
    getConstInputValue(&stat);\
    return stat;}\
    bool Un##plugName::setUniform(){\
    Q_ASSERT_X(checkShaderActive(),getDebugName().c_str(),"invalid shader id.");\
    bool stat;\
    SWW_GET(plugName::thisType,data,this,stat);\
    GLint loc = location();\
    glUploadFunc;\
    return true;}\
    bool Un##plugName::setUniform(GLuint programId,const std::string& varName){\
    bool stat;\
    SWW_GET(plugName::thisType,data,this,stat);\
    GLint loc = glGetUniformLocation(programId,varName.c_str());\
    glUploadFunc;\
    return true;}


SWW_UNPLUG_IMPL(BoolPlug,glUniform1i(loc,(int)data) )
SWW_UNPLUG_IMPL(FloatPlug,glUniform1f(loc,data) )
SWW_UNPLUG_IMPL(IntPlug,glUniform1i(loc,data) )
SWW_UNPLUG_IMPL(EnumPlug,glUniform1i(loc,data) )
SWW_UNPLUG_IMPL(V2fPlug,glUniform2fv(loc,1,(float*)&data) )
SWW_UNPLUG_IMPL(V3fPlug,glUniform3fv(loc,1,(float*)&data) )
SWW_UNPLUG_IMPL(V4fPlug,glUniform4fv(loc,1,(float*)&data) )
SWW_UNPLUG_IMPL(V2iPlug,glUniform2iv(loc,1,(int*)&data) )
SWW_UNPLUG_IMPL(V3iPlug,glUniform3iv(loc,1,(int*)&data) )
SWW_UNPLUG_IMPL(M33fPlug,glUniformMatrix3fv(loc, 1, GL_FALSE, &data[0][0]) )
SWW_UNPLUG_IMPL(M44fPlug,glUniformMatrix4fv(loc, 1, GL_FALSE, &data[0][0]) )
//SWW_UNPLUG_IMPL(GLTexturePlug,glUniformMatrix4fv(loc, 1, GL_FALSE, &data[0][0]) )

UnTexturePlug::UnTexturePlug(const SwwOpenGL::GLShaderProgramPtr& shader,
                             GLint texUnit,
                             const std::string& name,
                             SwwOpenGL::GLTexturePtr data,
                             GLenum adaptableTarget)
    :GLTexturePlug(name,data),IGLUniform(shader,name),_texUnit(texUnit),_adaptableTarget(adaptableTarget)
{
}

bool UnTexturePlug::justUpdate(){
    bool ok;
    getTexture(&ok);
    return ok;
}

bool UnTexturePlug::setUniform(){
    bool ok;
    const SwwOpenGL::GLTexturePtr& data = getTexture(&ok);
    if(!ok){
        SWW_ERR("set uniform failed. " << getDebugName());
        return false;
    }
    //if(getName() == "texA")_texUnit = 3;

    if(data->target() != _adaptableTarget){
        SWW_ERR("get texture failed. mismatch target type. " << getDebugName());

        SWW_DEBUG_VALUE(data->target());
        SWW_DEBUG_VALUE(_adaptableTarget);

        return false;
    }

    Q_ASSERT_X(checkShaderActive(),getName().c_str(),"not active.");
    data->bindUniform(programId(),getName(),_texUnit);
    return true;
}

bool UnTexturePlug::setUniform(GLuint programId,const std::string& varName){
    bool ok;
    const SwwOpenGL::GLTexturePtr& data = getTexture(&ok);
    if(!ok){
        SWW_ERR("set uniform failed. " << getDebugName());
        return false;
    }

    if(data->target() != _adaptableTarget){
        SWW_ERR("get texture failed. mismatch target type. " << getDebugName());
        return false;
    }

    Q_ASSERT(checkShaderActive());
    data->bindUniform(programId,varName,_texUnit);
    return true;
}

void UnTexturePlug::setAdaptableTarget(GLenum adaptableTarget){
    _adaptableTarget = adaptableTarget;
}


}//ns
