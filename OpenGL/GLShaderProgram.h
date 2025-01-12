#ifndef SWWGLSHADERPROGRAM_H
#define SWWGLSHADERPROGRAM_H

#include <GL/glew.h>
#include <QGLShaderProgram>
#include "Core/SwwAssert.h"

#include "Core/SwwTypeDef.h"
#include "OpenGL/GLTypeDefFwd.h"
#include "OpenGL/OpenGLAPI.h"
#include "OpenGL/RenderDevice.h"

namespace SwwOpenGL{
class SWWOPENGLSHARED_EXPORT GLShaderProgram : public QGLShaderProgram
{
public:
    GLShaderProgram();
    virtual ~GLShaderProgram();
    void bind();
    void release();

    //    void addUniform(GLUniform* uniform);
    //    void deleteExistingUniform(const std::string& name);//名前による削除

    void setUniformTexture(std::string varName,const GLTexturePtr& tex, int texunit);
    bool createShaderProgram(const std::string& vShader, const std::string& fShader);
    bool linkShaderProgram();

    void getShaderProgramInfo(GLenum programInterface,std::vector<std::string>& result)const; //programInterface eg.  GL_UNIFORM GL_PROGRAM_INPUT GL_PROGRAM_OUTPUT GL_UNIFORM_BLOCK
    void printUniforms()const;

    bool attachShaderFile(const std::string& shader,QGLShader::ShaderType type){return addShaderFromSourceFile(type,shader.c_str());}
    bool attachShaderFile(const Sww::Core::StringArrayPtr& shaders,QGLShader::ShaderType type);

    bool attachShaderFileAppendDefine(const std::string& shader,
                                      QGLShader::ShaderType type,
                                      const Sww::Core::StringArray& defines);


    bool hasUniform(const std::string& name)const;
    bool hasUniformBlock(const std::string& name)const;

    //    void bindSceneUbo(const RenderDevice& device);//シェーダーとuboをバインド　初期化時に１回走らせればよい
    //    void bindLightUbo(const RenderDevice& device);//シェーダーとuboをバインド　初期化時に１回走らせればよい

    bool bindUbo(const std::string& uboName,GLuint bindingPoint)const;//シェーダーとuboをバインド　初期化時に１回走らせればよい

    //void setModelMatrix(const Imath::M44f& m) const;

    void setValue(const std::string& varName,const Imath::V2f& value)const;
    void setValue(const std::string& varName,const Imath::V3f& value)const;
    void setValue(const std::string& varName,const Imath::V4f& value)const;
    void setValue(const std::string& varName,const Imath::V2i& value)const;
    void setValue(const std::string& varName,const Imath::V3i& value)const;
    void setValue(const std::string& varName,const Imath::V4i& value)const;
    void setValue(const std::string& varName,const Imath::M33f& value)const;
    void setValue(const std::string& varName,const Imath::M44f& value)const;

private:
    int _currentProgramId;
    void storeCurrentProgramId();


    //    bool bindSceneUbo();//シェーダーとuboをバインド　初期化時に１回走らせればよい
    //    bool bindLightUbo();//シェーダーとuboをバインド　初期化時に１回走らせればよい

    //    Sww::Core::StringArrayPtr _vertexShaders;
    //    Sww::Core::StringArrayPtr _geometryShaders;
    //    Sww::Core::StringArrayPtr _fragmentShaders;

};

inline void GLShaderProgram::bind()
{
    storeCurrentProgramId();
    QGLShaderProgram::bind();
    //    for(std::vector<GLUniform*>::iterator i =_uniforms.begin();i!=_uniforms.end();++i)
    //    {
    //        (*i)->setUniform();
    //    }
}

inline void GLShaderProgram::release()
{
    glUseProgram(_currentProgramId);
    //QGLShaderProgram::release();
}

}//ns

#endif // SWWGLSHADERPROGRAM_H
