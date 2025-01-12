#ifndef SWW_GLTEXTURE_H
#define SWW_GLTEXTURE_H

#include <GL/glew.h>
#include <GL/GL.h>
#include <string>

#include "OpenGL/OpenGLAPI.h"

//#include <mutex>

namespace SwwOpenGL{

class SWWOPENGLSHARED_EXPORT GLTexture
{
public:
#ifdef USE_OIIO
    enum SAVE_TYPE{
        SAVE_AS_FLOAT,
        SAVE_AS_HALF,
        SAVE_AS_UINT8,
        SAVE_AS_IS
    };
#endif

    GLTexture(int width , int height, int depth = 1,
              GLenum target = GL_TEXTURE_2D,
              GLint internalFormat = GL_RGBA32F,
              GLenum format = GL_RGBA,
              GLenum type = GL_FLOAT,
              GLint filter = GL_LINEAR,
              GLint wrap = GL_CLAMP_TO_EDGE);

    virtual ~GLTexture();

    void init();
    void resize(int width , int height, bool reset = false);
    void uploadData(const void *data)const;//外部からデータを送るだけ　保持しない
    void populateData(void* data);//外部からデータを送り、コピー、保持する 要解放
    void allocateData();//内部でデータ生成、保持する 要解放
    void uploadInternalData() const;//保持したデータをopenGLに反映
    const void* getData()const{return _data;}//保持したデータを返す

    GLuint texId()const{return _texId;}
    int width()const{return _width;}
    int height()const{return _height;}
    int depth()const{return _depth;}
    GLenum target()const{return _target;}
    GLenum type()const{return _type;}
    GLint internalFormat()const{return _internalFormat;}
    GLenum format()const{return _format;}
#ifdef USE_OIIO
    bool saveROI(const char *fileName, int xOrigin, int yOrigin, int roiWidth, int roiHeight);
    bool save(const char *fileName,SAVE_TYPE type = SAVE_AS_IS) const;
#endif
    int numChannels() const;

    void bind()const{glBindTexture(_target,_texId);}
    void release()const{glBindTexture(_target,0);}
    void bindUniform(GLuint programId,const std::string& varName,int texunit)const;//シェーダーに渡す

    void drawQuad();

protected:
    GLuint _texId;
    int _width,_height,_depth;
    GLenum _target; //GL_TEXTURE_2D,GL_TEXTURE_RECTANGLE,GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_2D_ARRAY
    GLint _internalFormat; //GL_RGBA32F GL_RGBA8
    GLenum _format; //GL_RGBA  GL_RGB  GL_DEPTH_COMPONENT
    GLenum _type; //GL_FLOAT  GL_UNSIGNED_BYTE
    GLint _filter; // GL_NEAREST GL_LINEAR
    GLint _wrap; // GL_CLAMP_TO_EDGE GL_CLAMP_TO_BORDER GL_CLAMP GL_REPEAT
    void* _data;

#ifdef USE_OIIO
    bool saveAsFloat(const char *fileName)const;
    bool saveAsUint8(const char *fileName)const;
#endif


    //    //このクラスはコピー禁止にしておく
    //    GLTexture(const  GLTexture&);            // コピーコンストラクタ
    //    GLTexture operator=(const  GLTexture&);  // 代入演算子オーバーロード
};





}//namespace
#endif // SWW_GLTEXTURE_H
