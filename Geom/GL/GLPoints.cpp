#include "Geom/GL/GLPoints.h"
#include "Geom/GL/GLVertexBufferObject.h"
#include "Geom/GL/GLIndexBufferObject.h"

#include "qglobal.h"

#include "Core/SwwAssert.h"
#include "Core/VectorHelper.h"


#include "OpenGL/SwwGLMacro.h" //header only
#include "Geom/GL/GLLocation.h"

namespace SwwGeom{

GLPoints::GLPoints():_generated(false),_boundsGenerated(false),
    _normalsGenerated(false),_uvsGenerated(false),_colorsGenerated(false),
    _attrIdNormal(0),_attrIdUv(0)
{
}

GLPoints::~GLPoints()
{
}

void GLPoints::clear(){
    SWW_CHECK_GL_CONTEXT;
    _attrs.clear();
}

void GLPoints::generate(Imath::V3f *vertices, size_t numVertices,
                        GLenum usage)
{
    Q_ASSERT(!_generated);

    GLVertexBufferObjectPtr attrPos =
            GLVertexBufferObjectPtr(new GLVertexBufferObject(locPos));
    attrPos->create((float*)vertices,3,
                    numVertices * 3 * sizeof(float),usage);

    _attrs.push_back(attrPos);

    _numVertices = numVertices;
    _usage = usage;
    _generated = true;
}

void GLPoints::update(Imath::V3f* vertices,size_t numVertices)const
{
    Q_ASSERT(_generated);

    GLVertexBufferObjectPtr attrPos = _attrs[0];
    attrPos->update((float*)vertices,numVertices * 3 * sizeof(float));

}

void GLPoints::generateBounds(const Imath::Box3f &iBounds,
                              GLenum usage)
{
    Q_ASSERT(_generated);
    //Q_ASSERT_X(_generated,"GLPoints","not generated.");

    float vertices[] = {
        iBounds.min.x, iBounds.min.y, iBounds.min.z,
        iBounds.max.x, iBounds.min.y, iBounds.min.z,
        iBounds.max.x, iBounds.max.y, iBounds.min.z,
        iBounds.min.x, iBounds.max.y, iBounds.min.z,
        iBounds.min.x, iBounds.min.y, iBounds.max.z,
        iBounds.max.x, iBounds.min.y, iBounds.max.z,
        iBounds.max.x, iBounds.max.y, iBounds.max.z,
        iBounds.min.x, iBounds.max.y, iBounds.max.z,
    };

    unsigned int indices[] = {
        0, 1, 2, 3,
        4, 5, 6, 7,
        0, 4, 1, 5, 2, 6, 3, 7
    };

    _vboBounds = GLVertexBufferObjectPtr(new GLVertexBufferObject(locPos));
    _vboBounds->create(vertices,3,8 * 3 * sizeof(float),usage);

    _iboBounds.reset(new GLIndexBufferObject());
    _iboBounds->create(indices,sizeof(indices),GL_STATIC_DRAW);

    _boundsGenerated = true;
}

void GLPoints::updateBounds(const Imath::Box3f &iBounds) const{
    Q_ASSERT(_boundsGenerated);
    float vertices[] = {
        iBounds.min.x, iBounds.min.y, iBounds.min.z,
        iBounds.max.x, iBounds.min.y, iBounds.min.z,
        iBounds.max.x, iBounds.max.y, iBounds.min.z,
        iBounds.min.x, iBounds.max.y, iBounds.min.z,
        iBounds.min.x, iBounds.min.y, iBounds.max.z,
        iBounds.max.x, iBounds.min.y, iBounds.max.z,
        iBounds.max.x, iBounds.max.y, iBounds.max.z,
        iBounds.min.x, iBounds.max.y, iBounds.max.z,
    };
    _vboBounds->update(vertices,8 * 3 * sizeof(float));
}

void GLPoints::generateNormals(Imath::V3f *normals, size_t numNormals)
{
    Q_ASSERT_X(_generated,"GLPoints","must generate() first");

    GLVertexBufferObjectPtr attr =
            GLVertexBufferObjectPtr(new GLVertexBufferObject(locNormal));
    attr->create((float*)normals,3,
                 numNormals * 3 * sizeof(float),
                 _usage);

    _attrIdNormal = _attrs.size();
    _attrs.push_back(attr);

    _normalsGenerated = true;
}

void GLPoints::generateUvs(Imath::V2f *uvs, size_t numUvs)
{
    Q_ASSERT_X(_generated,"GLPoints","must generate() first");
    GLVertexBufferObjectPtr attr =
            GLVertexBufferObjectPtr(new GLVertexBufferObject(locUv));
    attr->create((float*)uvs,2,
                 numUvs * 2 * sizeof(float),
                 _usage);


    _attrIdUv = _attrs.size();
    _attrs.push_back(attr);

    _uvsGenerated = true;
}

void GLPoints::generateColors(Imath::V3f *colors, size_t numColors)
{
    Q_ASSERT_X(_generated,"GLPoints","must generate() first");

    GLVertexBufferObjectPtr attr =
            GLVertexBufferObjectPtr(new GLVertexBufferObject(locColor));
    attr->create((float*)colors,3,
                 numColors * 3 * sizeof(float),
                 _usage);

    _attrIdColor = _attrs.size();
    _attrs.push_back(attr);
    _colorsGenerated = true;
}

void GLPoints::updateNormals(Imath::V3f *normals, size_t numNormals) const
{
    Q_ASSERT(_normalsGenerated);

    GLVertexBufferObjectPtr attrNormal = _attrs[_attrIdNormal];
    attrNormal->update((float*)normals,numNormals * 3 * sizeof(float));
}

void GLPoints::updateUvs(Imath::V2f* uvs,size_t numUvs)const{
    Q_ASSERT(_uvsGenerated);

    GLVertexBufferObjectPtr attrUvs = _attrs[_attrIdUv];
    attrUvs->update((float*)uvs,numUvs * 2 * sizeof(float));
}

void GLPoints::updateColors(Imath::V3f *colors, size_t numColors)const
{
    Q_ASSERT(_colorsGenerated);

    GLVertexBufferObjectPtr attrColors = _attrs[_attrIdColor];
    attrColors->update((float*)colors,numColors * 3 * sizeof(float));
}

void GLPoints::draw() const
{
    Q_ASSERT_X(_generated,"GLPoints","not generated.");


    //bind *******************************************
    SWW_VECTOR_FOREACH_CONST(GLVertexBufferObjectPtr,_attrs){
        (*i)->apply();
    }

    glDrawArrays(GL_POINTS,0,_numVertices);

    //clean up****************************************
    SWW_VECTOR_FOREACH_CONST(GLVertexBufferObjectPtr,_attrs){
        (*i)->unapply();
    }
    return;

}


void GLPoints::drawLegacy() const
{
    Q_ASSERT_X(_generated,"GLPoints","not generated.");

    //bind *******************************************

    const GLVertexBufferObjectPtr& vboTmp0 =  _attrs[0];
    glBindBuffer(GL_ARRAY_BUFFER, vboTmp0->vboId());
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    if(_normalsGenerated){
        const GLVertexBufferObjectPtr& vboTmp =  _attrs[_attrIdNormal];
        glBindBuffer(GL_ARRAY_BUFFER, vboTmp->vboId());
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, 0);
    }

    if(_uvsGenerated){
        const GLVertexBufferObjectPtr& vboTmp =  _attrs[_attrIdUv];
        glBindBuffer(GL_ARRAY_BUFFER, vboTmp->vboId());
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT,0,0);
    }

    if(_colorsGenerated){
        const GLVertexBufferObjectPtr& vboTmp =  _attrs[_attrIdColor];
        glBindBuffer(GL_ARRAY_BUFFER, vboTmp->vboId());
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_FLOAT, 0, 0);
    }

    //draw********************************************
    glDrawArrays(GL_POINTS,0,_numVertices);

    //clean up****************************************

    if (_normalsGenerated) glDisableClientState(GL_NORMAL_ARRAY);
    if (_uvsGenerated) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if (_colorsGenerated) glDisableClientState(GL_COLOR_ARRAY);

    // release vbo's
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

}//ns
