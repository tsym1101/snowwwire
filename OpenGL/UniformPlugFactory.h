#ifndef UNIFORMPLUGFACTORY_H
#define UNIFORMPLUGFACTORY_H

#include <GL/glew.h>
#include <string>
#include "OpenGL/GLTypeDefFwd.h"

#include "Core/SwwTypeDesc.h"
//#include <GL/GL.h>

#include "OpenGL/GLTexture.h"
#include <qglobal.h>
#include "OpenGL/PlugTypeGL.h"
#include "OpenGL/GLUtility.h"
#include "OpenGL/IGLUniform.h"

namespace Sww::Core{
class PlugBase;
}

namespace SwwOpenGL {

class UniformPlugFactory
{
public:

    UniformPlugFactory();
    Sww::Core::PlugBase *generate(const SwwOpenGL::GLShaderProgramPtr& shader, const std::string& name, GLenum type);
private:
    int _texUnitCounter;//複数枚テクスチャをアサインする場合、カウントアップ
};

///////////////////////////////////////////////////////

#define SWW_UNPLUG_DECLARE(plugName,defaultValue)\
    class SWWOPENGLSHARED_EXPORT Un##plugName : public Sww::Core::plugName ,public IGLUniform{\
    public:\
    Un##plugName(const SwwOpenGL::GLShaderProgramPtr& shader,const std::string& name,Sww::Core::plugName::thisType data = defaultValue);\
    virtual bool justUpdate();\
    virtual bool setUniform();\
    virtual bool setUniform(GLuint programId,const std::string& varName);};

SWW_UNPLUG_DECLARE(BoolPlug,false)
SWW_UNPLUG_DECLARE(FloatPlug,0.f)
SWW_UNPLUG_DECLARE(IntPlug,0)
SWW_UNPLUG_DECLARE(EnumPlug,0)
SWW_UNPLUG_DECLARE(V2fPlug,Imath::V2f(0.f,0.f))
SWW_UNPLUG_DECLARE(V3fPlug,Imath::V3f(0.f,0.f,0.f))
SWW_UNPLUG_DECLARE(V4fPlug,Imath::V4f(0.f,0.f,0.f,0.f))
SWW_UNPLUG_DECLARE(V2iPlug,Imath::V2i(0,0))
SWW_UNPLUG_DECLARE(V3iPlug,Imath::V3i(0,0,0))
SWW_UNPLUG_DECLARE(M33fPlug,Imath::M33f(1.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,1.f))
SWW_UNPLUG_DECLARE(M44fPlug,Imath::M44f(1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f))
;
//SWW_UNPLUG_DECLARE(GLTexturePlug,SwwOpenGL::GLTexturePtr())

class SWWOPENGLSHARED_EXPORT UnTexturePlug : public GLTexturePlug,public IGLUniform
{
public:
    UnTexturePlug(const SwwOpenGL::GLShaderProgramPtr& shader,
                  GLint texUnit,
                  const std::string& name,
                  SwwOpenGL::GLTexturePtr data = SwwOpenGL::GLTexturePtr(),
                  GLenum adaptableTarget = GL_TEXTURE_2D);
    virtual bool justUpdate();
    virtual bool setUniform();
    virtual bool setUniform(GLuint programId,const std::string& varName);

    void setTexUnit(GLint unit){_texUnit = unit;}
    GLint getTexUnit()const{return _texUnit;}

    //getTextureするときのテクスチャターゲットを指定してミスマッチを確認
    void setAdaptableTarget(GLenum adaptableTarget);//GL_TEXTURE_2D GL_TEXTURE_RECTANGLE GL_SAMPLER_CUBE etc

protected:
    GLint _texUnit;
    GLenum _adaptableTarget;
};

}// ns


#endif // UNIFORMPLUGFACTORY_H
