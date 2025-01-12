#ifndef GLLEGACYVBO_H
#define GLLEGACYVBO_H

#include <GL/glew.h>
#include <GL/GL.h>
#include <vector>

#include "OpenGL/OpenGLAPI.h"

namespace SwwOpenGL {

class SWWOPENGLSHARED_EXPORT GLLegacyVbo
{
public:
    GLLegacyVbo();
    ~GLLegacyVbo();

    void draw() const;

    /// @note accepted @c primType: GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP,
    /// GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES,
    /// GL_QUAD_STRIP, GL_QUADS and GL_POLYGON
    void genIndexBuffer(const std::vector<GLuint>&, GLenum primType);

    void genVertexBuffer(const std::vector<GLfloat>&);
    void genUvBuffer(const std::vector<GLfloat>&);
    void genNormalBuffer(const std::vector<GLfloat>&);
    void genColorBuffer(const std::vector<GLfloat>&);

    void clear();

private:
    GLuint _vertexBuffer,_uvBuffer,_normalBuffer, _indexBuffer, _colorBuffer;
    GLenum _primType;
    GLsizei _primNum;
};

}//ns

#endif // GLLEGACYVBO_H
