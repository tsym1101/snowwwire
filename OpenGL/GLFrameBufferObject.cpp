#include "GLFrameBufferObject.h"
#include "Core/SwwAssert.h"
#include "OpenGL/GLTexture.h"

#include <qglobal.h>

//#include "OpenGL/GLUtility.h"
#include "OpenGL/SwwGLMacro.h"
#include "Core/VectorHelper.h"

namespace SwwOpenGL{

GLFrameBufferObject::GLFrameBufferObject():_colorAttatchmentBase(GL_COLOR_ATTACHMENT0),
    _fboId(0),_oldFboId(0),_currentFboId(0)
{
    init();
}

GLFrameBufferObject::~GLFrameBufferObject()
{
    destroy();
}

void GLFrameBufferObject::init()
{
    SWW_CHECK_GL_CONTEXT;
    glGenFramebuffersEXT( 1,&_fboId );

    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &_numMaxAttachment);

}

void GLFrameBufferObject::begenViewport(){
    Q_ASSERT_X(_textures.size(),"fbo bind failed.","_textures empty.");
    glGetIntegerv(GL_VIEWPORT,_currentViewport);
    Q_ASSERT(!_textures.empty());
    const GLTexturePtr& tex0 = *(_textures.begin());
    glViewport(0,0,tex0->width(),tex0->height());
}

void GLFrameBufferObject::endViewport(){
    glViewport(_currentViewport[0],_currentViewport[1],_currentViewport[2],_currentViewport[3]);
}

void GLFrameBufferObject::drawQuad()
{
    Q_ASSERT(getTexture());
    getTexture()->drawQuad();
}

void GLFrameBufferObject::attatchRoutine(const GLTexturePtr &tex, GLenum attachment){
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &_oldFboId);
    CHECK_GL_ERRORS;
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, _fboId );
    CHECK_GL_ERRORS;
    _currentFboId = _fboId;
    switch(tex->target()) {
    case GL_TEXTURE_1D:
        glFramebufferTexture1DEXT( GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_1D, tex->texId(), 0 );
        break;
    case GL_TEXTURE_3D:
        glFramebufferTexture3DEXT( GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_3D, tex->texId(), 0, 0 );
        break;
    case GL_TEXTURE_2D_ARRAY_EXT:
        glFramebufferTextureLayerEXT( GL_FRAMEBUFFER_EXT, attachment, tex->texId(), 0, 0 );
        break;
    case GL_TEXTURE_CUBE_MAP:
        for (unsigned int i = 0; i < 6; ++i)
        {
            glFramebufferTexture2D( GL_FRAMEBUFFER, attachment,
                                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,tex->texId(), 0 );
            //CHECK_GL_ERRORS;
        }
        break;
    default: //GL_TEXTURE_2D, GL_TEXTURE_RECTANGLE
        glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, attachment, tex->target(), tex->texId(), 0 );
        break;
    }
    //glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _oldFboId);
    CHECK_GL_ERRORS;
    _currentFboId = _oldFboId;
}

void GLFrameBufferObject::attachTexture(const GLTexturePtr &tex, int attachmentPoint)
{
    SWW_CHECK_GL_CONTEXT;
    Q_ASSERT_X(_numMaxAttachment >= attachmentPoint,"attach tex.","max Attachment");

    GLenum attachment = _colorAttatchmentBase + attachmentPoint;

    attatchRoutine(tex,attachment);

    //CHECK_GL_ERRORS;

    Q_ASSERT(!_textures.contains(attachment));
    _textures[attachment] = tex;
    _activeDrawBuffers.push_back(attachment);


}

void GLFrameBufferObject::attachDepthTexture(const GLTexturePtr &tex){
    attatchRoutine(tex,GL_DEPTH_ATTACHMENT_EXT);
    _depthTex = tex;
}

void GLFrameBufferObject::destroy()
{
    if(_fboId)
    {
        glDeleteFramebuffersEXT(1, &_fboId);
        //if(_colorTexId)glDeleteTextures(1, &_colorTexId);
        //if(_colorBuffer)glDeleteRenderbuffersEXT(1,&_colorBuffer);
        //glDeleteRenderbuffersEXT(1,&_depthBuffer);
    }
}

void GLFrameBufferObject::activateDrawBuffer(int attachmentPoint) const
{
    glDrawBuffer(_colorAttatchmentBase + attachmentPoint);
}

void GLFrameBufferObject::activateAllDrawBuffers() const
{
    int numActiveDrawBuffers = _activeDrawBuffers.size();
    if(numActiveDrawBuffers == 0){
        glDrawBuffer(GL_NONE);
        //glReadBuffer(GL_NONE);
    }
    else{
        glDrawBuffers(_activeDrawBuffers.size(),&_activeDrawBuffers[0]);
    }
}

void GLFrameBufferObject::deactivateAllDrawBuffers() const
{
    if(_currentFboId==0){
        glDrawBuffer(GL_BACK);
        CHECK_GL_ERRORS;
    }else{
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        CHECK_GL_ERRORS;
    }

}


void GLFrameBufferObject::blitFramebuffer(GLFrameBufferObject *target,
                                          GLFrameBufferObject *source,
                                          GLbitfield buffers,
                                          GLenum filter)
{
    const GLTexturePtr& texSrc = source->getTexture();
    const GLTexturePtr& texSwwt = target->getTexture();

    if(!texSrc || !texSwwt)
    {
        SWW_ERR("blitFramebuffer failed. invalid FBO.");
        return;
    }

    const int sx0 = 0;
    const int sx1 = texSrc->width();
    const int sy0 = 0;
    const int sy1 = texSrc->height();

    const int tx0 = 0;
    const int tx1 = texSwwt->width();
    const int ty0 = 0;
    const int ty1 = texSwwt->height();

    glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, source->handle());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, target->handle());

    glBlitFramebufferEXT(sx0, sy0, sx1, sy1,
                         tx0, ty0, tx1, ty1,
                         buffers, filter);

    glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
}

bool GLFrameBufferObject::checkFramebufferStatus() const
{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);
    switch(status) {
    case GL_NO_ERROR:
    case GL_FRAMEBUFFER_COMPLETE_EXT:
        return true;
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
        SWW_ERR("GLFramebufferObject: Unsupported framebuffer format.");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
        SWW_ERR("GLFramebufferObject: Framebuffer incomplete attachment.");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
        SWW_ERR("GLFramebufferObject: Framebuffer incomplete, missing attachment.");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        SWW_ERR("GLFramebufferObject: Framebuffer incomplete, attached images must have same dimensions.");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        SWW_ERR("GLFramebufferObject: Framebuffer incomplete, attached images must have same format.");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        SWW_ERR("GLFramebufferObject: Framebuffer incomplete, missing draw buffer.");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
        SWW_ERR("GLFramebufferObject: Framebuffer incomplete, missing read buffer.");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT:
        SWW_ERR("GLFramebufferObject: Framebuffer incomplete, attachments must have same number of samples per pixel.");
        break;
    default:
        SWW_ERR("GLFramebufferObject: An undefined error has occurred: " << status);
        break;
    }
    return false;
}

void GLFrameBufferObject::bind()
{
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &_oldFboId);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fboId);
    _currentFboId = _fboId;
    //glDrawBuffers(_activeDrawBuffers.size(),&(_activeDrawBuffers[0]) );
    //SWW_DEBUG_VALUE(_activeDrawBuffers.size());
}

void GLFrameBufferObject::release()
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _oldFboId);
    _currentFboId = _oldFboId;
    //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    //glDrawBuffer(GL_BACK);

}


}//namespace
