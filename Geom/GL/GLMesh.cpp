#include <qglobal.h>

#include "Geom/GL/GLMesh.h"
#include "Geom/GL/GLVertexBufferObject.h"
#include "Geom/GL/GLIndexBufferObject.h"
#include "Core/SwwAssert.h"
#include "Core/VectorHelper.h"

#include "OpenGL/SwwGLMacro.h" //header only
#include "Geom/GL/GLLocation.h"

namespace SwwGeom{

GLMesh::GLMesh():_vaoId(0),_generated(false),_boundsGenerated(false),
    _normalsGenerated(false),_uvsGenerated(false),_colorsGenerated(false),_tangentsGenerated(false),
    _attrIdNormal(0),_attrIdUv(0),_attrIdColor(0),_attrIdTangent(0),
    _drawMode(GL_TRIANGLES)
  #ifdef SWW_INSTANCE_TEST
  ,_instanceGenerated(false),_attrIdInstance(0)
  #endif
{

}

GLMesh::~GLMesh()
{
    destroy();
}

void GLMesh::createTest()
{
    float positionData[] = {
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        0.0f,  0.8f, 0.0f };
    float colorData[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f };

    uint32_t indices[] = {
        0, 1, 2};


#ifdef SWW_DRAW_BY_VAO
    glGenVertexArrays(1,&_vaoId);
    glBindVertexArray(_vaoId);
#endif

    generate((Imath::V3f*)positionData,3,indices,3,GL_STATIC_DRAW);
    generateColors((Imath::V3f*)colorData,3);


#ifdef SWW_DRAW_BY_VAO
    glBindVertexArray(0);
#endif
}

void GLMesh::destroy()
{
#ifdef SWW_DRAW_BY_VAO
    glDeleteVertexArrays(1,&_vaoId);
#endif
}

void GLMesh::clear(){
    SWW_CHECK_GL_CONTEXT;
    _attrs.clear();
    _ibo.reset();

    _generated = false;
    _boundsGenerated = false;
    _normalsGenerated = false;
    _uvsGenerated = false;
    _colorsGenerated = false;
    _tangentsGenerated = false;
}

void GLMesh::generate(Imath::V3f *vertices, size_t numVertices,
                      uint32_t *indices, size_t numIndices,
                      GLenum usage)
{
    Q_ASSERT(!_generated);

    GLVertexBufferObjectPtr attrPos =
            GLVertexBufferObjectPtr(new GLVertexBufferObject(locPos));
    attrPos->create((float*)vertices,3,
                    numVertices * 3 * sizeof(float),usage);
    //attrPos->apply();

    _ibo.reset(new GLIndexBufferObject());
    _ibo->create(indices,numIndices * sizeof(unsigned int),usage);

    _attrs.push_back(attrPos);

    _numIndices = numIndices;
    _usage = usage;

    _generated = true;
}

void GLMesh::update(Imath::V3f* vertices,size_t numVertices)const
{
    Q_ASSERT(_generated);

    GLVertexBufferObjectPtr attrPos = _attrs[0];
    attrPos->update((float*)vertices,numVertices * 3 * sizeof(float));
}

void GLMesh::generateBounds(const Imath::Box3f &iBounds,
                            GLenum usage)
{
    Q_ASSERT(_generated);
    //Q_ASSERT_X(_generated,"GLMesh","not generated.");

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

void GLMesh::updateBounds(const Imath::Box3f &iBounds) const{
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
    //SWW_DEBUG("updateBounds[" << iBounds.min << ":" << iBounds.max << "]");
    _vboBounds->update(vertices,8 * 3 * sizeof(float));
}

void GLMesh::generateNormals(Imath::V3f *normals, size_t numNormals)
{
    Q_ASSERT_X(_generated,"GLMesh","must generate() first");

    GLVertexBufferObjectPtr attr =
            GLVertexBufferObjectPtr(new GLVertexBufferObject(locNormal));
    attr->create((float*)normals,3,
                 numNormals * 3 * sizeof(float),
                 _usage);

    _attrIdNormal = _attrs.size();
    _attrs.push_back(attr);

    _normalsGenerated = true;
}

void GLMesh::generateUvs(Imath::V2f *uvs, size_t numUvs)
{
    Q_ASSERT_X(_generated,"GLMesh","must generate() first");
    GLVertexBufferObjectPtr attr =
            GLVertexBufferObjectPtr(new GLVertexBufferObject(locUv));
    attr->create((float*)uvs,2,
                 numUvs * 2 * sizeof(float),
                 _usage);


    _attrIdUv = _attrs.size();
    _attrs.push_back(attr);

    _uvsGenerated = true;
}

void GLMesh::generateColors(Imath::V3f *colors, size_t numColors)
{
    Q_ASSERT_X(_generated,"GLMesh","must generate() first");

    GLVertexBufferObjectPtr attr =
            GLVertexBufferObjectPtr(new GLVertexBufferObject(locColor));
    attr->create((float*)colors,3,
                 numColors * 3 * sizeof(float),
                 _usage);

    _attrIdColor = _attrs.size();
    _attrs.push_back(attr);
    _colorsGenerated = true;
}

void GLMesh::generateTangents(Imath::V3f *tangents, size_t numTangents)
{
    Q_ASSERT_X(_generated,"GLMesh","must generate() first");

    GLVertexBufferObjectPtr attr =
            GLVertexBufferObjectPtr(new GLVertexBufferObject(locTangent));
    attr->create((float*)tangents,3,
                 numTangents * 3 * sizeof(float),
                 _usage);

    _attrIdTangent = _attrs.size();
    _attrs.push_back(attr);

    _tangentsGenerated = true;
}

#ifdef SWW_INSTANCE_TEST
void GLMesh::generateInstance(Imath::V3f *pos, size_t numPos)
{
    Q_ASSERT_X(_generated,"GLMesh","must generate() first");

    GLVertexBufferObjectPtr attr =
            GLVertexBufferObjectPtr(new GLVertexBufferObject(locInstancePos));
    attr->create((float*)pos,3,
                 numPos * 3 * sizeof(float),
                 _usage);

    glVertexAttribDivisor(locInstancePos,1); //instance特有の呼び出し

    _attrIdInstance = _attrs.size();
    _attrs.push_back(attr);
    _instanceGenerated = true;
    _numInstance = numPos;
}
#endif

void GLMesh::updateNormals(Imath::V3f *normals, size_t numNormals) const
{
    Q_ASSERT(_normalsGenerated);

    GLVertexBufferObjectPtr attrNormal = _attrs[_attrIdNormal];
    attrNormal->update((float*)normals,numNormals * 3 * sizeof(float));
}

void GLMesh::updateUvs(Imath::V2f* uvs,size_t numUvs)const{
    Q_ASSERT(_uvsGenerated);

    GLVertexBufferObjectPtr attrUvs = _attrs[_attrIdUv];
    attrUvs->update((float*)uvs,numUvs * 2 * sizeof(float));
}

void GLMesh::updateColors(Imath::V3f *colors, size_t numColors)const
{
    Q_ASSERT(_colorsGenerated);

    GLVertexBufferObjectPtr attrColors = _attrs[_attrIdColor];
    attrColors->update((float*)colors,numColors * 3 * sizeof(float));
}

void GLMesh::updateIndices(const unsigned int *indices, size_t numIndices) const{
    Q_ASSERT_X(_generated,"GLMesh","must generate() first");

    _ibo->update(indices,numIndices * sizeof(unsigned int));
}

void GLMesh::updateTangents(Imath::V3f *tangents, size_t numTangents) const
{
    Q_ASSERT(_tangentsGenerated);

    GLVertexBufferObjectPtr attrTangents = _attrs[_attrIdTangent];
    attrTangents->update((float*)tangents,numTangents * 3 * sizeof(float));
}

void GLMesh::apply() const{
    for(auto i:_attrs){
        i->apply();
    }
    _ibo->apply();
}

void GLMesh::unapply() const{
    for(auto i:_attrs){
        i->unapply();
    }
    _ibo->unapply();
}

void GLMesh::draw() const
{
#ifdef SWW_DRAW_BY_VAO
    glBindVertexArray(_vaoId);
    glDrawElements(
                GL_POLYGON,      // mode
                3,    // numIndices
                GL_UNSIGNED_INT,   // type
                (void*)0           // element array buffer offset
                );
    glBindVertexArray(0);

#else

    Q_ASSERT_X(_generated,"GLMesh","not generated.");

    //    SWW_VECTOR_FOREACH_CONST(GLVertexBufferObjectPtr,_attrs){
    //        (*i)->apply();
    //    }
    //    _ibo->apply();
    apply();

#ifndef SWW_INSTANCE_TEST
    //draw********************************************
    glDrawElements(
                _drawMode,      // mode
                _numIndices,       // numIndices
                GL_UNSIGNED_INT,   // type
                (void*)0           // element array buffer offset
                );

#else
    if(_instanceGenerated){

        glDrawElementsInstanced(
                    _drawMode,
                    _numIndices,
                    GL_UNSIGNED_INT,
                    (void*)0,
                    _numInstance
                    );
    }else{
        glDrawElements(
                    _drawMode,      // mode
                    _numIndices,       // numIndices
                    GL_UNSIGNED_INT,   // type
                    (void*)0           // element array buffer offset
                    );
    }

#endif

    //    //clean up****************************************
    //    SWW_VECTOR_FOREACH_CONST(GLVertexBufferObjectPtr,_attrs){
    //        (*i)->unapply();
    //    }
    //    _ibo->unapply();

    unapply();

#endif

}


void GLMesh::drawLegacy() const
{
    Q_ASSERT_X(_generated,"GLMesh","not generated.");

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

    _ibo->apply();

    //draw********************************************
    glDrawElements(
                _drawMode,      // mode
                _numIndices,       // numIndices
                GL_UNSIGNED_INT,   // type
                (void*)0           // element array buffer offset
                );
    //clean up****************************************

    if (_normalsGenerated) glDisableClientState(GL_NORMAL_ARRAY);
    if (_uvsGenerated) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if (_colorsGenerated) glDisableClientState(GL_COLOR_ARRAY);

    // release vbo's
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    _ibo->unapply();

}

//void GLMesh::drawBounds(const Imath::M44f &mvp)
//{
//    if(!_boundsGenerated){
//        SWW_DEBUG("bounds not generated.");
//        return;
//    }

//    _shaderBounds->bind();

//    GLuint loc = glGetUniformLocation(_shaderBounds->programId(), "MVP");
//    glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]);

//    _vboBounds->apply();
//    _iboBounds->apply();

//    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
//    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(4*sizeof(unsigned int)));
//    glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (GLvoid*)(8*sizeof(unsigned int)));

//    _vboBounds->unapply();
//    _iboBounds->unapply();

//    _shaderBounds->release();
//}

//void GLMesh::setVertexAttr(const GLVertexBufferObject &attr)
//{

//}

}//ns
