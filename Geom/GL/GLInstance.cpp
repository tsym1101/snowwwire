#include "GLInstance.h"
#include "Geom/GL/GLVertexBufferObject.h"
#include "Geom/GL/GLIndexBufferObject.h"

#include "qglobal.h"

#include "Core/SwwAssert.h"
#include "Core/VectorHelper.h"

#include "OpenGL/SwwGLMacro.h" //header only
#include "Geom/GL/GLLocation.h"

#include "Geom/GL/GLMesh.h"

namespace SwwGeom {

GLInstance::GLInstance():_baseMesh(0),_instanceGenerated(false),_numInstance(0)
{    
}

void GLInstance::generateInstance(GLMesh* baseMesh, Imath::M44f *mat, size_t numPoint, GLenum usage)
{
    _baseMesh = baseMesh;
    _usage = usage;

    _instanceGenerated = true;
    _numInstance = numPoint;

    GLVertexBufferObjectPtr attr = GLVertexBufferObjectPtr(new GLVertexBufferObject(locInstance));
    attr->create((float*)mat,16,
                 numPoint * sizeof(Imath::M44f),
                 _usage);

    _attrInstance = attr;
}

void GLInstance::updateInstance(Imath::M44f *mat, size_t numPoint)
{
    Q_ASSERT(_instanceGenerated);
    _attrInstance->update((float*)mat,numPoint * sizeof(Imath::M44f));
    _numInstance = numPoint;
}

void GLInstance::generateMagnitude(float *value, size_t numPoint){
    Q_ASSERT(_instanceGenerated);
    GLVertexBufferObjectPtr attr = GLVertexBufferObjectPtr(new GLVertexBufferObject(locMagnitude));
    attr->create(value,1,
                 numPoint * sizeof(float),
                 _usage);

    glVertexAttribDivisor(locMagnitude,1);//インスタンス特有の呼び出し

    _attrMagnitude = attr;
}

void GLInstance::updateMagnitude(float *value, size_t numPoint){
    Q_ASSERT(_instanceGenerated);
    _attrInstance->update((float*)value,numPoint * sizeof(Imath::M44f));
}

void GLInstance::draw()const{

    Q_ASSERT_X(valid(),"GLInstance","invlaid.");
    Q_ASSERT_X(_baseMesh->isGenerated(),"GLMesh","not generated.");

    _attrInstance->apply();
    if(_attrMagnitude)_attrMagnitude->apply();
    CHECK_GL_ERRORS;

    _baseMesh->apply();

    CHECK_GL_ERRORS;

    GLenum drawMode = _baseMesh->getDrawMode();
    size_t numIndices = _baseMesh->getNumIndices();

    glDrawElementsInstanced(
                drawMode,
                numIndices,
                GL_UNSIGNED_INT,
                (void*)0,
                _numInstance
                );

    _attrInstance->unapply();
    if(_attrMagnitude)_attrMagnitude->unapply();
    _baseMesh->unapply();

}

bool GLInstance::valid()const{
    return (_baseMesh || _instanceGenerated);
}

void GLInstance::clear(){
    _baseMesh = 0;
    //_attrInstance.reset();
    _instanceGenerated = false;
}



}//ns
