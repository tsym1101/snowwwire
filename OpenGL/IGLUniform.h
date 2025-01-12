#ifndef SWW_IGLUNIFORM_H
#define SWW_IGLUNIFORM_H

#include <string>
#include <GL/glew.h>
#include <GL/GL.h>
#include <ImathMatrix.h>
#include "OpenGL/GLTexture.h"
#include <qglobal.h>
#include "OpenGL/PlugTypeGL.h"
#include "OpenGL/GLUtility.h"

namespace  SwwOpenGL {

class SWWOPENGLSHARED_EXPORT IGLUniform
{
public:
    IGLUniform(const SwwOpenGL::GLShaderProgramPtr& shader,const std::string& name);

    virtual bool justUpdate() = 0;
    virtual bool setUniform() = 0;
    //virtual bool setUniform()const = 0;
    virtual bool setUniform(unsigned int programId,const std::string& varName)
    {return setUniform();}

    const std::string& getUniformName()const{return _name;}

protected:
    void init(const SwwOpenGL::GLShaderProgramPtr& shader,const std::string& name);
    GLint location()const{return _location;}
    GLint programId()const{return _programId;}
    bool checkShaderActive() const;

private:
    GLuint _programId;
    GLint _location;
    std::string _name;//uniform変数名
};

















}//ns
#endif // SWW_IGLUNIFORM_H
