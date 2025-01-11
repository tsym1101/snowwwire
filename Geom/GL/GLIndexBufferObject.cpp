#include "GLIndexBufferObject.h"
#include "Core/SwwAssert.h"

namespace SwwGeom {

GLIndexBufferObject::GLIndexBufferObject()
    :_iboId(0)
{
}

GLIndexBufferObject::~GLIndexBufferObject()
{
    destroy();
}

void GLIndexBufferObject::create(const unsigned int* indices, size_t dataSize, GLenum usage)
{
    glGenBuffers(1, &_iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 dataSize,
                 indices,
                 usage);

    _dataSize = dataSize;
    _usage = usage;
}

void GLIndexBufferObject::destroy()
{
    glDeleteBuffers(1,&_iboId);
}

void GLIndexBufferObject::apply()const
{
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _iboId );
}

void GLIndexBufferObject::unapply()const
{
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void GLIndexBufferObject::update(const unsigned int* data,size_t dataSize)
{
    if(dataSize > _dataSize){
        destroy();
        create(data,dataSize,_usage);
        return;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboId);

#if 0
    glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, data);
#else
    void* ptr = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER,0,dataSize,
                                 GL_MAP_WRITE_BIT|GL_MAP_UNSYNCHRONIZED_BIT);

    memcpy(ptr,data,dataSize);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

#endif
}

}//ns
