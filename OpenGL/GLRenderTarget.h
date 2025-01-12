#ifndef GLRENDERTARGET_H
#define GLRENDERTARGET_H

//#include "OpenGL/GLFrameBufferObject.h"
//#include "GLTexture.h"
#include <GL/glew.h>
#include <GL/GL.h>
#include "OpenGL/OpenGLAPI.h"

#include "OpenGL/GLTypeDefFwd.h"

namespace SwwOpenGL{

class SWWOPENGLSHARED_EXPORT GLRenderTarget
{
public:
    GLRenderTarget(int width, int height, GLenum target = GL_TEXTURE_2D ,
                   int numColorAttachment = 1 ,
                   GLint filter = GL_LINEAR);
    ~GLRenderTarget();

    void init(int width, int height, GLenum target,int numColorAttachment ,GLint filter);
    void destroy();

    void resize(int width, int height);

    void bind();
    void release();
    void activateAllDrawBuffers() const;
    void deactivateAllDrawBuffers() const;

    const GLTexturePtr& getColorTexBuffer(int index = 0)const;
    const GLTexturePtr& getDepthTexBuffer()const;

    GLuint colorTexId()const;
    GLuint depthTexId()const;
    GLenum target()const;

    int width()const;
    int height()const;
#if 0
    bool saveColor(const char* fileName)const;
    bool saveDepth(const char* fileName)const;
#endif

    bool checkFrameBufferStatus();

    const GLFrameBufferObject* fboPtr()const{
        if(_fbo)return _fbo.get();
        else return NULL;
    }

private:
    GLFrameBufferObjectPtr _fbo;
    std::vector<GLTexturePtr> _colorBuffers;
    GLTexturePtr _depthBuffer;


    //このクラスはコピー禁止にしておく
    GLRenderTarget(const  GLRenderTarget&);            // コピーコンストラクタ
    GLRenderTarget operator=(const  GLRenderTarget&);  // 代入演算子オーバーロード
};

} //namespace
#endif // GLRENDERTARGET_H
