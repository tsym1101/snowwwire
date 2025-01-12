#include "GLUniformBufferObjectHelper.h"
#include "OpenGL/GLShaderProgram.h"
#include "OpenGL/SwwGLMacro.h"

namespace SwwOpenGL {

GLUniformBufferObjectHelper::GLUniformBufferObjectHelper(const GLShaderProgramPtr& shader) {
    SWW_CHECK_GL_CONTEXT;

    getUboMeta(shader, _uboMetaList);

    int numUbo = _uboMetaList.size();
    if (numUbo) {
        _uboArray = std::make_unique<GLuint[]>(numUbo);
        glGenBuffers(numUbo, _uboArray.get());
        for (int i = 0; i < numUbo; i++) {
            glBindBuffer(GL_UNIFORM_BUFFER, _uboArray[i]);
            glBufferData(GL_UNIFORM_BUFFER, _uboMetaList[i]._dataSize, 0, GL_DYNAMIC_DRAW);
        }

        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    printUboMeta();
}

GLUniformBufferObjectHelper::~GLUniformBufferObjectHelper() {
    SWW_CHECK_GL_CONTEXT;

    int numUbo = _uboMetaList.size();
    if (numUbo) {
        glDeleteBuffers(numUbo, _uboArray.get());
    }
}

bool GLUniformBufferObjectHelper::getUboMetaByName(const std::string& name, UboMeta& result) const {
    for (const auto& m : _uboMetaList) {
        if (m._name == name) {
            result = m;
            return true;
        }
    }
    return false;
}

void GLUniformBufferObjectHelper::getUboMeta(const GLShaderProgramPtr& shader, UboMetaArray& uboMetaList) {
    int numUbo = -1;
    glGetProgramiv(shader->programId(), GL_ACTIVE_UNIFORM_BLOCKS, &numUbo);

    for (int i = 0; i < numUbo; i++) {
        char uboName[256];
        int uboNameLength = -1;

        glGetActiveUniformBlockName(shader->programId(), i, sizeof(uboName), &uboNameLength, uboName);
        GLuint blockIndex = glGetUniformBlockIndex(shader->programId(), uboName);

        GLint numActiveInUbo;
        GLint uboDataSize;

        glGetActiveUniformBlockiv(shader->programId(), blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uboDataSize);
        glGetActiveUniformBlockiv(shader->programId(), blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numActiveInUbo);

        auto indices = std::make_unique<GLuint[]>(numActiveInUbo);
        auto offsets = std::make_unique<GLint[]>(numActiveInUbo);
        auto sizes = std::make_unique<GLsizei[]>(numActiveInUbo);

        glGetActiveUniformBlockiv(shader->programId(), blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (GLint*)indices.get());
        glGetActiveUniformsiv(shader->programId(), numActiveInUbo, indices.get(), GL_UNIFORM_OFFSET, offsets.get());
        glGetActiveUniformsiv(shader->programId(), numActiveInUbo, indices.get(), GL_UNIFORM_SIZE, sizes.get());

        UboMeta uboMetaTmp;
        uboMetaTmp._name = uboName;
        uboMetaTmp._blockIndex = blockIndex;
        uboMetaTmp._dataSize = uboDataSize;
        uboMetaTmp._bindingPoint = i + 1;
        uboMetaTmp._myIndex = i;

        for (int k = 0; k < numActiveInUbo; k++) {
            UniformMetaInUbo uniformMeta;
            char nameUniform[256];
            int nameUniformLength = -1, num = -1;
            GLenum type = GL_ZERO;

            glGetActiveUniform(shader->programId(), indices[k], sizeof(nameUniform), &nameUniformLength, &num, &type, nameUniform);

            uniformMeta._name = nameUniform;
            uniformMeta._index = indices[k];
            uniformMeta._offset = offsets[k];
            uniformMeta._size = sizes[k];
            uboMetaTmp._uniformMetaList.push_back(uniformMeta);
        }

        uboMetaList.push_back(uboMetaTmp);
    }
}

void GLUniformBufferObjectHelper::printUboMeta() const {
    SWW_DEBUG("                                             ");
    SWW_DEBUG("shader ubo meta ******************************");
    for (const auto& m : _uboMetaList) {
        SWW_DEBUG("name[" << m._name << "] ===============" << "\t\t\t");
        SWW_DEBUG("blockIndex   : " << m._blockIndex << "\t\t\t");
        SWW_DEBUG("dataSize     : " << m._dataSize << "\t\t\t");
        SWW_DEBUG("bindingPoint : " << m._bindingPoint << "\t\t\t");
        SWW_DEBUG("ubo : " << _uboArray[m._myIndex] << "\t\t\t");

        SWW_DEBUG("uniform variables\t\t\t");
        for (const auto& metaInUbo : m._uniformMetaList) {
            SWW_DEBUG("\tname[" << metaInUbo._name << "] ***\t\t\t");
            SWW_DEBUG("\tindex      : " << metaInUbo._index << "\t\t\t");
            SWW_DEBUG("\toffset     : " << metaInUbo._offset << "\t\t\t");
            SWW_DEBUG("\tsize       : " << metaInUbo._size << "\t\t\t");
        }
    }
    SWW_DEBUG("                                             ");
}

} // namespace SwwOpenGL
