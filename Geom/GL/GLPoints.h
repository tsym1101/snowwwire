#ifndef GL_POINTS_H
#define GL_POINTS_H

#include "Core/VectorHelper.h"

#include "Geom/GL/GeomGLTypeDefFwd.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <ImathVec.h>
#include <ImathMatrix.h>
#include <ImathBox.h>

namespace SwwGeom {

class  GLPoints
{
public:
    GLPoints();
    ~GLPoints();

    void generate(Imath::V3f* vertices,size_t numVertices,
                  GLenum usage = GL_STATIC_DRAW);
    void generateNormals(Imath::V3f* normals,size_t numNormals);
    void generateUvs(Imath::V2f* uvs,size_t numUvs);
    void generateColors(Imath::V3f* colors,size_t numColors);

    void update(Imath::V3f* vertices,size_t numVertices)const;
    void updateNormals(Imath::V3f* normals,size_t numNormals)const;
    void updateUvs(Imath::V2f* uvs,size_t numUvs)const;
    void updateColors(Imath::V3f* colors,size_t numColors)const;

    void generateBounds(const Imath::Box3f& iBounds, GLenum usage = GL_STATIC_DRAW);
    void updateBounds(const Imath::Box3f& iBounds)const;

    void clear();

    virtual void draw() const;
    virtual void drawLegacy() const;


private:
    std::vector<GLVertexBufferObjectPtr> _attrs;
    size_t _numVertices;

    Imath::Box3f _bounds;
    GLVertexBufferObjectPtr _vboBounds;
    GLIndexBufferObjectPtr _iboBounds;

    GLenum _usage;

    bool _generated;
    bool _boundsGenerated;
    bool _normalsGenerated;
    bool _uvsGenerated;
    bool _colorsGenerated;

    unsigned short _attrIdNormal;//生成した順番を保持
    unsigned short _attrIdUv;
    unsigned short _attrIdColor;

};


}//ns
#endif // GL_POINTS_H
