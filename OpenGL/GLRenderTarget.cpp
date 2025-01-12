#include "GLRenderTarget.h"
#include <iostream>

#include "OpenGL/GLTexture.h"
#include "OpenGL/GLFrameBufferObject.h"

#include "Core/SwwAssert.h"

#include "OpenGL/SwwGLMacro.h"

namespace SwwOpenGL{
GLRenderTarget::GLRenderTarget(int width,int height,GLenum target,int numColorAttachment ,GLint filter)
{

    init(width,height,target,numColorAttachment,filter);
}

GLRenderTarget::~GLRenderTarget()
{
}

void GLRenderTarget::init(int width , int height , GLenum target, int numColorAttachment, GLint filter)
{
    _fbo.reset(new GLFrameBufferObject());
    GLint numMaxAttachment;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &numMaxAttachment);
    for(int i = 0;i < numColorAttachment;i++){
        if(i > numMaxAttachment){
            SWW_ERR("over max color attchment." << numMaxAttachment);
            break;
        }
        GLTexturePtr colorBuffer(new GLTexture(width,height,1,target,GL_RGBA32F,GL_RGBA,
                                               GL_FLOAT,
                                               filter));
        _fbo->attachTexture(colorBuffer,i);
        _colorBuffers.push_back(colorBuffer);
    }

    _depthBuffer.reset(new GLTexture(width,height,1,target,
                                     GL_DEPTH_COMPONENT32F,GL_DEPTH_COMPONENT,
                                     GL_FLOAT,
                                     filter)
                       );
    _fbo->attachDepthTexture(_depthBuffer);

    Q_ASSERT(_fbo->checkFramebufferStatus());
}

GLuint GLRenderTarget::colorTexId()const{
    if(_colorBuffers.empty()){
        SWW_ERR("empty color buffer.");
        return 0;
    }
    return _colorBuffers[0]->texId();
}
GLuint GLRenderTarget::depthTexId()const{return _depthBuffer->texId();}
GLenum GLRenderTarget::target()const{
    if(_colorBuffers.empty()){
        SWW_ERR("empty color buffer.");
        return 0;
    }
    return _colorBuffers[0]->target();
}
int GLRenderTarget::width()const {
    if(_colorBuffers.empty()){
        if(_depthBuffer)return _depthBuffer->width();
        else Q_ASSERT(0);
    }
    return _colorBuffers[0]->width();
}
int GLRenderTarget::height()const {
    if(_colorBuffers.empty()){
        if(_depthBuffer)return _depthBuffer->height();
        else Q_ASSERT(0);
    }
    return _colorBuffers[0]->height();
}

void GLRenderTarget::bind(){
    _fbo->bind();
    CHECK_GL_ERRORS;
}

void GLRenderTarget::release(){
    _fbo->release();
    CHECK_GL_ERRORS;
}

void GLRenderTarget::activateAllDrawBuffers() const
{
    _fbo->activateAllDrawBuffers();
}

void GLRenderTarget::deactivateAllDrawBuffers() const
{
    //CHECK_GL_ERRORS;
    _fbo->deactivateAllDrawBuffers();
    //CHECK_GL_ERRORS;
}

bool GLRenderTarget::checkFrameBufferStatus(){return _fbo->checkFramebufferStatus();}

void GLRenderTarget::resize(int width, int height)
{
    for(int i = 0;i < _colorBuffers.size();i++){
        _colorBuffers[i]->resize(width,height,true);
    }
    _depthBuffer->resize(width,height,true);
}

#if 0
bool GLRenderTarget::saveColor(const char *fileName) const
{
    return _colorBuffers[0]->save(fileName);
}

bool GLRenderTarget::saveDepth(const char *fileName) const
{
    return _depthBuffer->save(fileName);
}
#endif

const GLTexturePtr& GLRenderTarget::getColorTexBuffer(int index)const{
    Q_ASSERT_X(index < _colorBuffers.size(),"getColorTexBuffer","out of range.");
    return _colorBuffers[index];
}

const GLTexturePtr& GLRenderTarget::getDepthTexBuffer()const{
    return _depthBuffer;
}

} //namespace
