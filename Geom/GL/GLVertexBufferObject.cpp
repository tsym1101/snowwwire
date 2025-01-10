#include "GLVertexBufferObject.h"
#include <qglobal.h>
#include "Core/SwwAssert.h"
#include "OpenGL/SwwGLMacro.h" //header only

namespace SwwGeom {

GLVertexBufferObject::GLVertexBufferObject(int location)
    :_location(location),_vboId(0),_attrSize(0),_dataSize(0),_usage(GL_STATIC_DRAW),_typeIsMat(false)
{

}

GLVertexBufferObject::~GLVertexBufferObject()
{
    destroy();
}

void GLVertexBufferObject::create(const float* data,
                                  int attrSize, size_t dataSize, GLenum usage)
{
    if(attrSize <= 4){
        _attrSize = attrSize;
        glGenBuffers(1, &_vboId);
        glBindBuffer(GL_ARRAY_BUFFER, _vboId);
        glBufferData(GL_ARRAY_BUFFER, dataSize,data,usage);

        CHECK_GL_ERRORS;

        _dataSize = dataSize;
        _usage = usage;
    }
    else if(attrSize == 16){
        glGenBuffers(1, &_vboId);
        glBindBuffer(GL_ARRAY_BUFFER, _vboId);
        glBufferData(GL_ARRAY_BUFFER, dataSize,data,usage);

        _attrSize = 4;//内部的にvec4として扱われる

        //instance特有の呼び出し
        for(int i = 0;i < 4;i++){
            glEnableVertexAttribArray(_location + i);
            glVertexAttribPointer(
                        _location  + i,             // attribute
                        _attrSize,     // size
                        GL_FLOAT,           // type
                        GL_FALSE,           // normalized?
                        sizeof(float) * 4 * 4,                  // stride
                        (void*)(sizeof(float) * 4 * i)            // array buffer offset
                        );
            glVertexAttribDivisor(_location + i,1);
        }

        _typeIsMat = true;

    }else{
        Q_ASSERT_X(0,"create VBO failed.","invalid paramator.");
    }


}

void GLVertexBufferObject::update(const float* data,size_t dataSize)
{
    Q_ASSERT_X(_attrSize,"vbo update","vbo not created.");

    if(dataSize > _dataSize){
        destroy();
        create(data,_attrSize,dataSize,_usage);
        return;

        SWW_WARN("vbo data size changed.");
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vboId);

#if 0
    glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, data);
#else
    void* ptr = glMapBufferRange(GL_ARRAY_BUFFER,0,dataSize,
                                 GL_MAP_WRITE_BIT|GL_MAP_UNSYNCHRONIZED_BIT);
    //    void* ptr = glMapBufferRange(GL_ARRAY_BUFFER,0,dataSize,
    //                                 GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_BUFFER_BIT );

    memcpy(ptr,data,dataSize);
    glUnmapBuffer(GL_ARRAY_BUFFER);

#endif
}

void GLVertexBufferObject::destroy()
{
    glDeleteBuffers(1,&_vboId);
    _vboId = 0;
}

void GLVertexBufferObject::apply()const
{
    if(!_typeIsMat){
        glEnableVertexAttribArray(_location);
        glBindBuffer(GL_ARRAY_BUFFER,_vboId);
        glVertexAttribPointer(
                    _location,             // attribute
                    _attrSize,     // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );
    }
    else{
        glBindBuffer(GL_ARRAY_BUFFER,_vboId);

        CHECK_GL_ERRORS;

        for(int i = 0;i < 4 ;i++){
            glEnableVertexAttribArray(_location + i);
            glVertexAttribPointer(
                        _location  + i,             // attribute
                        _attrSize,     // size
                        GL_FLOAT,           // type
                        GL_FALSE,           // normalized?
                        sizeof(GLfloat) * 4 * 4,                  // stride
                        (void*)(sizeof(float) * 4 * i)            // array buffer offset
                        );

        }
    }
}

void GLVertexBufferObject::unapply() const
{
    if(!_typeIsMat){
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glDisableVertexAttribArray(_location);
    }
    else{
        glBindBuffer(GL_ARRAY_BUFFER,0);
        for(int i = 0;i < 4 ;i++){
            glDisableVertexAttribArray(_location + i);
        }
    }

}

}//ns
