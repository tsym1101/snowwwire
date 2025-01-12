#ifndef SWW_GLUNIFORMBUFFEROBJECTHELPER_H
#define SWW_GLUNIFORMBUFFEROBJECTHELPER_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <memory> // std::unique_ptr

#include "OpenGL/GLTypeDefFwd.h"
#include "Core/SwwAssert.h"
#include "OpenGL/OpenGLAPI.h"

namespace SwwOpenGL {
class GLShaderProgram;

struct UniformMetaInUbo {
    std::string _name;
    GLuint _index;
    GLuint _offset;
    GLsizei _size;
};

struct SWWOPENGLSHARED_EXPORT UboMeta {
    GLuint _bindingPoint; // UBO側のバインド用インデックス
    std::string _name;
    uint32_t _myIndex; // 生成順
    GLuint _blockIndex; // シェーダ内のインデックス
    GLuint _dataSize;
    std::vector<UniformMetaInUbo> _uniformMetaList;
};

class SWWOPENGLSHARED_EXPORT GLUniformBufferObjectHelper {
public:
    GLUniformBufferObjectHelper(const SwwOpenGL::GLShaderProgramPtr& shader);
    ~GLUniformBufferObjectHelper();

    template<class STRUCT_T>
    void upload(const STRUCT_T& value, const UboMeta& target) const;

    void printUboMeta() const;
    bool getUboMetaByName(const std::string& name, UboMeta& result) const;
    const UboMetaArray& getUboMetaList() const { return _uboMetaList; }

    static void getUboMeta(const GLShaderProgramPtr& shader, UboMetaArray& uboMetaList);

private:
    UboMetaArray _uboMetaList;
    std::unique_ptr<GLuint[]> _uboArray;
};

template <class STRUCT_T>
void GLUniformBufferObjectHelper::upload(const STRUCT_T& value, const UboMeta& target) const {
    Q_ASSERT_X(sizeof(STRUCT_T) == target._dataSize, "failed upload to ubo.", target._name.c_str());

    glBindBufferBase(GL_UNIFORM_BUFFER, target._bindingPoint, _uboArray[target._myIndex]);

    STRUCT_T* ptr = (STRUCT_T*)glMapBufferRange(
        GL_UNIFORM_BUFFER,
        0, // dataOffset
        sizeof(STRUCT_T),
        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT
    );

    *ptr = value;

    glUnmapBuffer(GL_UNIFORM_BUFFER);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0);
}

} // namespace SwwOpenGL

#endif // SWW_GLUNIFORMBUFFEROBJECTHELPER_H
