#ifndef GLLINES_H
#define GLLINES_H

#include "Core/VectorHelper.h"
#include "Geom/GL/GeomGLTypeDefFwd.h"
#include "Core/SwwTypeDef.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <ImathVec.h>
#include <ImathMatrix.h>
#include <ImathBox.h>

#include <memory> // std::unique_ptr

namespace SwwGeom {

class GLLines {
public:
    GLLines();
    ~GLLines();

    void generate(Imath::V3f* vertices, size_t numVertices,
                  const Sww::Core::IntArrayPtr& countsArray,
                  GLenum usage = GL_STATIC_DRAW);
    void generateNormals(Imath::V3f* normals, size_t numNormals);
    void generateUvs(Imath::V2f* uvs, size_t numUvs);
    void generateColors(Imath::V3f* colors, size_t numColors);
    void generateAlphas(float* alphas, size_t numAlphas);

    void update(Imath::V3f* vertices, size_t numVertices) const;
    void setCountsArray(const Sww::Core::IntArrayPtr& countsArray);
    void updateNormals(Imath::V3f* normals, size_t numNormals) const;
    void updateUvs(Imath::V2f* uvs, size_t numUvs) const;
    void updateColors(Imath::V3f* colors, size_t numColors) const;
    void updateAlphas(float* alphas, size_t numAlphas) const;

    void generateBounds(const Imath::Box3f& iBounds, GLenum usage = GL_STATIC_DRAW);
    void updateBounds(const Imath::Box3f& iBounds) const;

    void clear();

    virtual void draw() const;
    virtual void drawLegacy() const;

private:
    std::vector<GLVertexBufferObjectPtr> _attrs;

    Imath::Box3f _bounds;
    GLVertexBufferObjectPtr _vboBounds;
    GLIndexBufferObjectPtr _iboBounds;

    std::unique_ptr<int[]> _offsetsArray;
    Sww::Core::IntArrayPtr _countsArray;

    GLenum _usage;

    bool _generated;
    bool _boundsGenerated;
    bool _normalsGenerated;
    bool _uvsGenerated;
    bool _colorsGenerated;
    bool _alphasGenerated;

    unsigned short _attrIdNormal; // 生成した順番を保持
    unsigned short _attrIdUv;
    unsigned short _attrIdColor;
    unsigned short _attrIdAlpha;
};

} // namespace SwwGeom
#endif // GLLINES_H
