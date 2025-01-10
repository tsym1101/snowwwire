#ifndef GLINDEXBUFFEROBJECT_H
#define GLINDEXBUFFEROBJECT_H

#include <GL/glew.h>
#include <GL/gl.h>

namespace SwwGeom {
class GLIndexBufferObject
{
public:
    GLIndexBufferObject();
    ~GLIndexBufferObject();

    void create(const unsigned int *indices,size_t dataSize ,
                GLenum usage = GL_STATIC_DRAW);
    void destroy();

    void update(const unsigned int* data,size_t dataSize);

    void apply()const;
    void unapply()const;

    GLuint iboId()const{return _iboId;}

private:
    GLuint _iboId;
    size_t _dataSize;
    GLenum _usage;
};

} //ns

#endif // GLINDEXBUFFEROBJECT_H
