#ifndef GLMESH_H
#define GLMESH_H

#include "Core/VectorHelper.h"

#include "Geom/GL/GeomGLTypeDefFwd.h"
#include <GL/glew.h>
#include <GL/gl.h>

#include <ImathVec.h>
#include <ImathMatrix.h>
#include <ImathBox.h>

namespace SwwGeom {

class  GLMesh
{
public:
    GLMesh();
    ~GLMesh();

    void createTest();
    void generate(Imath::V3f* vertices,size_t numVertices,
                  unsigned int* indices,size_t numIndices,
                  GLenum usage = GL_STATIC_DRAW);
    void generateNormals(Imath::V3f* normals,size_t numNormals);
    void generateUvs(Imath::V2f* uvs,size_t numUvs);
    void generateColors(Imath::V3f* colors,size_t numColors);
    void generateTangents(Imath::V3f* tangents,size_t numTangents);

    void update(Imath::V3f* vertices,size_t numVertices)const;
    void updateNormals(Imath::V3f* normals,size_t numNormals)const;
    void updateUvs(Imath::V2f* uvs,size_t numUvs)const;
    void updateColors(Imath::V3f* colors,size_t numColors)const;
    void updateIndices(const unsigned int *indices, size_t numIndices)const;
    void updateTangents(Imath::V3f* tangents,size_t numTangents)const;

    void generateBounds(const Imath::Box3f& iBounds, GLenum usage = GL_STATIC_DRAW);
    void updateBounds(const Imath::Box3f& iBounds)const;

    void clear();

    void destroy();
    virtual void draw() const;
    virtual void drawLegacy() const;
    //virtual void drawBounds(const Imath::M44f &mvp);

    void setDrawMode(GLenum mode);
    //void setVertexAttr(const GLVertexBufferObject& attr);

    void apply()const;
    void unapply()const;

    GLenum getDrawMode()const{return _drawMode;}
    unsigned int getNumIndices()const{return _numIndices;}

    bool isGenerated()const{return _generated;}

//#define SWW_INSTANCE_TEST
#ifdef SWW_INSTANCE_TEST
    void generateInstance(Imath::V3f* pos , size_t numPos);
    //void updateInstance(Imath::V3f* pos,size_t numPos)const;
#endif

protected:
    std::vector<GLVertexBufferObjectPtr> _attrs;
    GLIndexBufferObjectPtr _ibo;
    size_t _numVertex;
    GLuint _vaoId;//unused

    Imath::Box3f _bounds;
    GLVertexBufferObjectPtr _vboBounds;
    GLIndexBufferObjectPtr _iboBounds;


    unsigned int _numIndices;
    GLenum _usage;
    GLenum _drawMode;

    bool _generated;
    bool _boundsGenerated;
    bool _normalsGenerated;
    bool _uvsGenerated;
    bool _colorsGenerated;
    bool _tangentsGenerated;

    unsigned short _attrIdNormal;//生成した順番を保持
    unsigned short _attrIdUv;
    unsigned short _attrIdColor;
    unsigned short _attrIdTangent;

#ifdef SWW_INSTANCE_TEST
    bool _instanceGenerated;
    unsigned short _attrIdInstance;
    int _numInstance;
#endif

    //GLuint _iboId;
    //GLuint _vboId[2];
};

inline void GLMesh::setDrawMode(GLenum mode){
    _drawMode = mode;
}


}//ns
#endif // GLMESH_H
