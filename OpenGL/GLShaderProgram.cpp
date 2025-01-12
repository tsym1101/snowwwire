#include "GLShaderProgram.h"
#include "Core/VectorHelper.h"

#include "OpenGL/GLTexture.h"
#include "OpenGL/SwwGLMacro.h"

#include "OpenGL/GLUniformBufferObjectHelper.h"
#include "OpenGL/GLGlobal.h"

#include <QFile>

namespace SwwOpenGL{
GLShaderProgram::GLShaderProgram()
{

}

GLShaderProgram::~GLShaderProgram()
{
    //destoryUniforms();
}

void GLShaderProgram::storeCurrentProgramId(){
    glGetIntegerv(GL_CURRENT_PROGRAM,(GLint*)&_currentProgramId);
}

void GLShaderProgram::setUniformTexture(std::string varName,const GLTexturePtr& tex, int texunit)
{
    glActiveTexture(GL_TEXTURE0 + texunit);
    glBindTexture(tex->target(), tex->texId());

    GLint linked;
    glGetProgramiv(programId(), GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        SWW_ERR("Error: setTextureUnit needs program to be linked.");
        exit(1);
    }
    GLint id = glGetUniformLocation(programId(), varName.c_str());
    if (id == -1) {
        return;
    }
    glUniform1i(id, texunit);
    glActiveTexture(GL_TEXTURE0);
}

bool GLShaderProgram::attachShaderFile(const Sww::Core::StringArrayPtr& shaders,QGLShader::ShaderType type){
    //removeAllShaders();
    const Sww::Core::StringArray& array = *(shaders.get());
    SWW_VECTOR_FOREACH_CONST(std::string,array){
        if(!addShaderFromSourceFile(type,(*i).c_str()))
        {
            SWW_ERR("add shader failed." << (*i));
            return false;
        }
    };
    return true;
}

bool GLShaderProgram::attachShaderFileAppendDefine(const std::string &shader,
                                                   QGLShader::ShaderType type,
                                                   const Sww::Core::StringArray &defines){

    QFile file(shader.c_str());

    if (!file.open(QIODevice::ReadOnly))//読込のみでオープンできたかチェック
    {
        SWW_ERR("invalid.");
        return false;
    }

    QTextStream in(&file);
    QString src = in.readAll();

    if(!defines.empty()){
        QRegExp regVersion("^\\s*\\#version\\s+\\d+\\s*\\n");
        regVersion.setMinimal(true);

        QString defineStr;
        for(const auto str :defines){
            defineStr += QString(str.c_str()) + "\n";
        }

        int pos = regVersion.indexIn(src);
        if(pos > -1){
            QString rep = regVersion.cap(0) + defineStr;
            src.replace(regVersion,rep);
        }else{
            src.insert(0,defineStr);
        }

        //SWW_DEBUG_VALUE(src.toStdString());
    }

    return addShaderFromSourceCode(type,src);

}

bool GLShaderProgram::linkShaderProgram()
{
    SWW_CHECK_GL_CONTEXT;

    if(!link())
    {
        SWW_ERR("link shader failed.");
        removeAllShaders();
        return false;
    }

    //    //*************************************
    //    //もしuboを使っていたら、バインド
    //    bindUbo(GLGlobal::CAMERA_UBO_NAME,GLGlobal::CAMERA_UBO_BINDING_POINT);
    //    bindUbo(GLGlobal::LIGHT_UBO_NAME,GLGlobal::LIGHT_UBO_BINDING_POINT);

    return true;
}

bool GLShaderProgram::createShaderProgram(const std::string &vShader, const std::string &fShader)
{
    removeAllShaders();

    if(!addShaderFromSourceFile(QGLShader::Vertex,vShader.c_str()))
    {
        SWW_ERR("add vShader failed." << vShader);
        return false;
    }

    if(!addShaderFromSourceFile(QGLShader::Fragment,fShader.c_str()))
    {
        SWW_ERR("add fShader failed." << fShader);
        return false;
    }

    if(!linkShaderProgram())return false;

    return true;
}

void GLShaderProgram::getShaderProgramInfo(GLenum programInterface,std::vector<std::string>& result) const
{
    //programInterface eg.  GL_UNIFORM GL_PROGRAM_INPUT GL_PROGRAM_OUTPUT GL_UNIFORM_BLOCK
    int numActiveAttribs;
    glGetProgramInterfaceiv(programId(), programInterface,GL_ACTIVE_RESOURCES, &numActiveAttribs);

    for (int i = 0; i < numActiveAttribs; i++)
    {
        int identifier_length;
        char identifier[256];            //Where GL will write the variable name
        glGetProgramResourceName(programId(), programInterface,i, 256, &identifier_length, identifier);

        unsigned int inputIndex = glGetProgramResourceIndex(programId(),programInterface, identifier);
        if(inputIndex != i)
        {
            SWW_WARN("shader resource index is NOT consecutive.");
        }

        result.push_back(identifier);
    }
}

void GLShaderProgram::printUniforms() const{
    std::vector<std::string> uniforms;
    getShaderProgramInfo(GL_UNIFORM,uniforms);
    for(int i = 0;i < uniforms.size();i++){
        SWW_DEBUG_VALUE(uniforms[i]);
    }
}

bool GLShaderProgram::hasUniform(const std::string &name) const{
    std::vector<std::string> uniforms;
    getShaderProgramInfo(GL_UNIFORM,uniforms);

    for(int i = 0;i < uniforms.size();i++){
        if(name==uniforms[i])return true;
    }
    return false;
}

bool GLShaderProgram::hasUniformBlock(const std::string &name) const{
    std::vector<std::string> ub;
    getShaderProgramInfo(GL_UNIFORM_BLOCK,ub);
    for(int i = 0;i < ub.size();i++){
        if(name==ub[i])return true;
    }
    return false;
}

bool GLShaderProgram::bindUbo(const std::string &uboName, GLuint bindingPoint) const{
    if(!hasUniformBlock(uboName)){
        SWW_WARN("ubo not found in shader program. " << uboName);
        return true;
    }

    GLuint blockIndex = glGetUniformBlockIndex(programId(),uboName.c_str());
    glUniformBlockBinding(programId(),blockIndex,bindingPoint);
    CHECK_GL_ERRORS;

    return true;
}

void GLShaderProgram::setValue(const std::string& varName,const Imath::V2f& value)const{
    GLint location = glGetUniformLocation(programId(),varName.c_str());
    glUniform2fv(location,1,(float*)&value);
}

void GLShaderProgram::setValue(const std::string& varName,const Imath::V3f& value)const{
    GLint location = glGetUniformLocation(programId(),varName.c_str());
    glUniform3fv(location,1,(float*)&value);
}

void GLShaderProgram::setValue(const std::string& varName,const Imath::V4f& value)const{
    GLint location = glGetUniformLocation(programId(),varName.c_str());
    glUniform4fv(location,1,(float*)&value);
}

void GLShaderProgram::setValue(const std::string& varName,const Imath::V2i& value)const{
    GLint location = glGetUniformLocation(programId(),varName.c_str());
    glUniform2iv(location,1,(int*)&value);
}

void GLShaderProgram::setValue(const std::string& varName,const Imath::V3i& value)const{
    GLint location = glGetUniformLocation(programId(),varName.c_str());
    glUniform3iv(location,1,(int*)&value);
}

void GLShaderProgram::setValue(const std::string& varName,const Imath::V4i& value)const{
    GLint location = glGetUniformLocation(programId(),varName.c_str());
    glUniform4iv(location,1,(int*)&value);
}

void GLShaderProgram::setValue(const std::string& varName,const Imath::M33f& value)const{
    GLint location = glGetUniformLocation(programId(),varName.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

void GLShaderProgram::setValue(const std::string& varName,const Imath::M44f& value)const{
    GLint location = glGetUniformLocation(programId(),varName.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

//void GLShaderProgram::setModelMatrix(const Imath::M44f& m)const{
//    GLGlobal::MODEL_MATRIX_VARIABLE_NAME;
//    GLint location = glGetUniformLocation(programId(),GLGlobal::MODEL_MATRIX_VARIABLE_NAME);
//    glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]);
//}

}//ns
