#ifndef SWWFRAMEBUFFEROBJECT_H
#define SWWFRAMEBUFFEROBJECT_H

#include <GL/glew.h>
#include <GL/GL.h>
#include <vector>
//#include <map>
#include <QMap>

#include "OpenGL/GLTypeDefFwd.h"
#include <qglobal.h>

#include "OpenGL/OpenGLAPI.h"

namespace SwwOpenGL{
class SWWOPENGLSHARED_EXPORT GLFrameBufferObject
{
public:
    GLFrameBufferObject();
    ~GLFrameBufferObject();

    void bind();
    void release();

    void begenViewport(); //fboのサイズにビューポートに合わせる
    void endViewport(); //ビューポートを元に戻す

    inline GLuint handle()const{return _fboId;}   

    static void blitFramebuffer(GLFrameBufferObject *target, GLFrameBufferObject *source,
                                GLbitfield buffers = GL_COLOR_BUFFER_BIT,GLenum filter = GL_NEAREST);

    bool checkFramebufferStatus()const;

    void attachTexture(const GLTexturePtr& tex,int attachmentPoint = 0);
    void attachDepthTexture(const GLTexturePtr& tex);
    const GLTexturePtr& getTexture(int attachmentPoint = 0);
    const GLTexturePtr& getTextuDepthTexture(){return _depthTex;}
    void activateDrawBuffer(int attachmentPoint)const;
    void activateAllDrawBuffers() const;
    void deactivateAllDrawBuffers() const;

    GLint getFboId()const{return _fboId;}
    GLenum getColorAttachmentBase()const {return _colorAttatchmentBase;}

    void drawQuad();

private:
    void init();
    void destroy();

    void attatchRoutine(const GLTexturePtr& tex,GLenum attachment);

    GLuint _fboId;
    GLint _currentFboId;
    GLint _oldFboId;

    GLint _currentViewport[4];

    QMap<GLenum,GLTexturePtr> _textures;
    //std::vector<GLTexturePtr> _textures;
    std::vector<GLenum> _activeDrawBuffers;

    GLTexturePtr _depthTex;
    GLint _numMaxAttachment;
    GLenum _colorAttatchmentBase; //GL_COLOR_ATTACHMENT0 or GL_COLOR_ATTACHMENT0_EXT

    //このクラスはコピー禁止にしておく
    GLFrameBufferObject(const  GLFrameBufferObject&);            // コピーコンストラクタ
    GLFrameBufferObject operator=(const  GLFrameBufferObject&);  // 代入演算子オーバーロード
};



inline const GLTexturePtr& GLFrameBufferObject::getTexture(int  attachmentPoint)
{
    //Q_ASSERT(_textures.size() > attachmentPoint);
    return _textures.value(attachmentPoint); //該当しなければ0をかえす
}

}//namespace

#endif // SWWFRAMEBUFFEROBJECT_H
