#include "GLLegacyVbo.h"

#include "Core/SwwAssert.h"


namespace SwwOpenGL {

GLLegacyVbo::GLLegacyVbo():
    _vertexBuffer(0),
    _normalBuffer(0),
    _indexBuffer(0),
    _colorBuffer(0),
    _primType(GL_POINTS),
    _primNum(0)
{
}

GLLegacyVbo::~GLLegacyVbo() { clear(); }

void
GLLegacyVbo::draw() const
{
    if (_primNum == 0 || !glIsBuffer(_indexBuffer) || !glIsBuffer(_vertexBuffer)) {
        SWW_ERR("request to render empty or uninitialized buffer");
        return;
    }

    const bool usesUvBuffer = glIsBuffer(_uvBuffer);
    const bool usesColorBuffer = glIsBuffer(_colorBuffer);
    const bool usesNormalBuffer = glIsBuffer(_normalBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    if (usesUvBuffer) {
        glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2,GL_FLOAT,0,0);
    }

    if (usesColorBuffer) {
        glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_FLOAT, 0, 0);
    }

    if (usesNormalBuffer) {
        glEnableClientState(GL_NORMAL_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
        glNormalPointer(GL_FLOAT, 0, 0);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
    glDrawElements(_primType, _primNum, GL_UNSIGNED_INT, 0);

    // disable client-side capabilities
    if (usesUvBuffer) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if (usesColorBuffer) glDisableClientState(GL_COLOR_ARRAY);
    if (usesNormalBuffer) glDisableClientState(GL_NORMAL_ARRAY);

    // release vbo's
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void
GLLegacyVbo::genIndexBuffer(const std::vector<GLuint>& v, GLenum primType)
{
    // clear old buffer
    if (glIsBuffer(_indexBuffer) == GL_TRUE) glDeleteBuffers(1, &_indexBuffer);

    // gen new buffer
    glGenBuffers(1, &_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
    if (glIsBuffer(_indexBuffer) == GL_FALSE) throw "Error: Unable to create index buffer";

    // upload data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * v.size(), &v[0], GL_STATIC_DRAW); // upload data
    if (GL_NO_ERROR != glGetError()) throw "Error: Unable to upload index buffer data";

    // release buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    _primNum = GLsizei(v.size());
    _primType = primType;
}

void
GLLegacyVbo::genVertexBuffer(const std::vector<GLfloat>& v)
{
    if (glIsBuffer(_vertexBuffer) == GL_TRUE) glDeleteBuffers(1, &_vertexBuffer);

    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    if (glIsBuffer(_vertexBuffer) == GL_FALSE) throw "Error: Unable to create vertex buffer";

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * v.size(), &v[0], GL_STATIC_DRAW);
    if (GL_NO_ERROR != glGetError()) throw "Error: Unable to upload vertex buffer data";

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
GLLegacyVbo::genUvBuffer(const std::vector<GLfloat>& v)
{
    if (glIsBuffer(_uvBuffer) == GL_TRUE) glDeleteBuffers(1, &_uvBuffer);

    glGenBuffers(1, &_uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
    if (glIsBuffer(_uvBuffer) == GL_FALSE) throw "Error: Unable to create uv buffer";

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * v.size(), &v[0], GL_STATIC_DRAW);
    if (GL_NO_ERROR != glGetError()) throw "Error: Unable to upload uv buffer data";

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
GLLegacyVbo::genNormalBuffer(const std::vector<GLfloat>& v)
{
    if (glIsBuffer(_normalBuffer) == GL_TRUE) glDeleteBuffers(1, &_normalBuffer);

    glGenBuffers(1, &_normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
    if (glIsBuffer(_normalBuffer) == GL_FALSE) throw "Error: Unable to create normal buffer";

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * v.size(), &v[0], GL_STATIC_DRAW);
    if (GL_NO_ERROR != glGetError()) throw "Error: Unable to upload normal buffer data";

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
GLLegacyVbo::genColorBuffer(const std::vector<GLfloat>& v)
{
    if (glIsBuffer(_colorBuffer) == GL_TRUE) glDeleteBuffers(1, &_colorBuffer);

    glGenBuffers(1, &_colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
    if (glIsBuffer(_colorBuffer) == GL_FALSE) throw "Error: Unable to create color buffer";

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * v.size(), &v[0], GL_STATIC_DRAW);
    if (GL_NO_ERROR != glGetError()) throw "Error: Unable to upload color buffer data";

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
GLLegacyVbo::clear()
{
    if (glIsBuffer(_indexBuffer) == GL_TRUE) glDeleteBuffers(1, &_indexBuffer);
    if (glIsBuffer(_vertexBuffer) == GL_TRUE) glDeleteBuffers(1, &_vertexBuffer);
    if (glIsBuffer(_uvBuffer) == GL_TRUE) glDeleteBuffers(1, &_uvBuffer);
    if (glIsBuffer(_colorBuffer) == GL_TRUE) glDeleteBuffers(1, &_colorBuffer);
    if (glIsBuffer(_normalBuffer) == GL_TRUE) glDeleteBuffers(1, &_normalBuffer);

    _primType = GL_POINTS;
    _primNum = 0;
}

}//ns
