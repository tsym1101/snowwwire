#ifndef SWW_GLUNIFORMBUFFEROBJECT_H
#define SWW_GLUNIFORMBUFFEROBJECT_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>
#include "Core/SwwAssert.h"

#include <QMap>
#include "OpenGL/OpenGLAPI.h"
namespace SwwOpenGL {

class SWWOPENGLSHARED_EXPORT GLUniformBufferObject
{
public:
    GLUniformBufferObject(const std::string& name,GLuint dataSize);
    ~GLUniformBufferObject();

    template<class STRUCT_T>
    void upload(const STRUCT_T &value);

    GLuint getUbo()const{return _ubo;}
    bool valid()const;

    const std::string& getName()const{return _name;}
    GLuint getBindingPoint()const{return _bindingPoint;}

    //void restore()const;


private:
    GLuint _ubo;
    GLuint _bindingPoint;//ubo側のバインド用インデックス シェーダー間でシェアするときに用いる
    //GLuint _blockIndex; //shader内のインデックス
    std::string _name;
    GLuint _dataSize;

    static QMap<int,GLUniformBufferObject*> _bindingPointTable;
    void getAvailableBindingPoint();
    //    static GLuint _currentUbo;
    //    GLuint _oldUbo;
};

template <class STRUCT_T>
void GLUniformBufferObject::upload(const STRUCT_T &value)
{
    Q_ASSERT_X(sizeof(STRUCT_T) == _dataSize,"failed upload to ubo.",_name.c_str());



    //glBindBuffer(GL_UNIFORM_BUFFER,_ubo);

    glBindBufferBase( GL_UNIFORM_BUFFER, _bindingPoint,_ubo);
    //    SWW_DEBUG_VALUE(_bindingPoint);
    //    SWW_DEBUG_VALUE(_ubo);

    STRUCT_T* ptr = (STRUCT_T*)glMapBufferRange(
                GL_UNIFORM_BUFFER,
                0, //dataOffset
                sizeof(STRUCT_T),
                GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT
                );

    *ptr = value;

    glUnmapBuffer(GL_UNIFORM_BUFFER);
    //glBindBufferBase(GL_UNIFORM_BUFFER,0,0);

    //    _oldUbo = _currentUbo;
    //    _currentUbo = _ubo;

    //glBindBuffer(GL_UNIFORM_BUFFER,0);
}



}//ns

#endif // SWW_GLUNIFORMBUFFEROBJECT_H
