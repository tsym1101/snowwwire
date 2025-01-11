#ifndef GLVertexBufferObject_H
#define GLVertexBufferObject_H

#include <GL/glew.h>
#include <GL/gl.h>


namespace SwwGeom {

class GLVertexBufferObject
{
public:
    GLVertexBufferObject(int location);
    ~GLVertexBufferObject();

    void create(const float *data,int attrSize ,size_t dataSize ,
                GLenum usage = GL_STATIC_DRAW);

    void update(const float* data, size_t dataSize);
    void destroy();

    void apply()const;
    void unapply()const;

    GLuint vboId()const{return _vboId;}
    GLuint location()const{return _location;}

private:
    int _attrSize;//スカラーなら1,vector３だったら3  openGLの仕様で5以上はNG
    int _location;
    GLuint _vboId;
    size_t _dataSize;
    GLenum _usage;

    bool _typeIsMat;//mat4を扱うときはvec4*4として扱わなければならない　フラグを立てて場合わけする
};

}//ns

#endif // GLVertexBufferObject_H
