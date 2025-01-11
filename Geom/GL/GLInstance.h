#ifndef GLINSTANCE_H
#define GLINSTANCE_H

//#include "Core/VectorHelper.h"

//#include "Geom/GL/GeomGLTypeDefFwd.h"

//#include <GL/glew.h>
//#include <GL/gl.h>

//#include <ImathVec.h>
//#include <ImathMatrix.h>
//#include <ImathBox.h>

#include "Geom/GL/GLMesh.h"

namespace SwwGeom {

class GLMesh;

class GLInstance
{
public:
    GLInstance();

    void generateInstance(GLMesh* baseMesh,
                          Imath::M44f* mat , size_t numPoint,
                          GLenum usage = GL_STATIC_DRAW);
    void updateInstance(Imath::M44f* mat,size_t numPoint);

    void generateMagnitude(float* value , size_t numPoint);
    void updateMagnitude(float* value , size_t numPoint);


    virtual void draw() const;

    bool valid()const;
    void clear();

private:
    GLMesh* _baseMesh;
    GLenum _usage; //インスタンス配列のusage

    bool _instanceGenerated;
    int _numInstance;

    GLVertexBufferObjectPtr _attrInstance;
    GLVertexBufferObjectPtr _attrMagnitude;

};
}//ns

#endif // GLINSTANCE_H
