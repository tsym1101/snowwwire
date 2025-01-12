#-------------------------------------------------
#
# Project created by QtCreator 2018-01-16T12:41:49
#
#-------------------------------------------------

# Check if the config file exists
! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

include(../external/glCommon.pri)
include(../external/ilmbaseCommon.pri)
#include(../external/openimageioCommon.pri)
#include(../external/opencvCommon.pri)

#QT       -= gui
QT       += opengl

TARGET = SwwGeom
TEMPLATE = lib

DEFINES += SWWGEOM_LIBRARY

LIBS += -L$${DESTDIR} -lSwwUtil -lSwwCore

#poly2tri*************************************************
INCLUDEPATH +=  $${SWW_LIBS_ROOT}/poly2tri/include

CONFIG(debug, debug|release) {
        LIBS +=\
        $${SWW_LIBS_ROOT}/poly2tri/lib/poly2trid.lib
}
CONFIG(release, debug|release) {
        LIBS +=\
        $${SWW_LIBS_ROOT}/poly2tri/lib/poly2tri.lib
}
#**********************************************************

SOURCES += \
    Camera.cpp \
    #GLFrameBufferObject.cpp \
    #GLGlobal.cpp \
    #GLImageBufferObject.cpp \
    #GLLegacyVbo.cpp \
#    #GLNode.cpp \
#    #GLObjectI.cpp \
    #GLRenderTarget.cpp \
#    #GLShaderHelper.cpp \
    #GLShaderProgram.cpp \
    #GLTexture.cpp \
    #GLUniformBufferObject.cpp \
    #GLUniformBufferObjectHelper.cpp \
#    #GLUniformPlug.cpp \
    #GLUtility.cpp \
#    #GLViewer.cpp \
#    #GLXForm.cpp \
#    #Poly2triHelper.cpp \
    #RenderDevice.cpp \
#    #Renderer.cpp \
#    #ShadingGroup.cpp \
    OrthCamera.cpp \
    PlugTypeGeom.cpp \
    Shape.cpp \
#    PlugTypeGeom.cpp \
    Mesh.cpp \
#    Camera.cpp \
    Bounds.cpp \
#    GeomEntry.cpp \
#    OrthCamera.cpp \
    #NdQuad.cpp \
    Quad.cpp \
#    TileCamera.cpp \
    Points.cpp \
    Lines.cpp \
    TileCamera.cpp \
    Transform.cpp \
    #NdPointLight.cpp \
    Light.cpp \
    Cube.cpp \
    #NdCube.cpp \
    #NdExtractVP.cpp \
    SpotLight.cpp \
    #NdDirectionalLight.cpp \
    DirectionalLight.cpp \
    #NdSpotLight.cpp \
    Instance.cpp \
    #NdInstance.cpp \
    #NdAxisAlignedPointsToBoundingMesh.cpp \
    #NdTransformGeo.cpp \
    #NdExtract2dImageFrom3dImage.cpp \
    ArrayProperty.cpp \
    TransFunc.cpp \
    TransFuncIntensity.cpp \
    TransFuncMappingKey.cpp

HEADERS +=\
    Camera.h \
    #GLDrawRoutine.h \
    #GLFrameBufferObject.h \
    #GLGlobal.h \
    #GLImageBufferObject.h \
    #GLLegacyVbo.h \
#    #GLNode.h \
#    #GLObjectI.h \
    #GLRenderTarget.h \
#    #GLShaderHelper.h \
    #GLShaderProgram.h \
    #GLTexture.h \
    #GLTypeDefFwd.h \
    #GLUniformBufferObject.h \
    #GLUniformBufferObjectHelper.h \
#    #GLUniformPlug.h \
    #GLUtility.h \
#    #GLViewer.h \
#    #GLXForm.h \
    #MathDef.h \
#    #Poly2triHelper.h \
    #RenderDevice.h \
#    #Renderer.h \
#    #ShadingGroup.h \
    #SwwGLMacro.h \
    GeomAPI.h \
    OrthCamera.h \
    PlugTypeGeom.h \
    Shape.h \
#    PlugTypeGeom.h \
    Mesh.h \
#    Camera.h \
    Bounds.h \
#    GeomEntry.h \
#    OrthCamera.h \
    #NdQuad.h \
    Quad.h \
#    TileCamera.h \
    Points.h \
    Lines.h \
    Image.h \
    TileCamera.h \
    Transform.h \
    #NdPointLight.h \
    Light.h \
    Cube.h \
    #NdCube.h \
    #NdExtractVP.h \
    SpotLight.h \
    #NdDirectionalLight.h \
    DirectionalLight.h \
    #NdSpotLight.h \
    Instance.h \
    #NdInstance.h \
    #NdAxisAlignedPointsToBoundingMesh.h \
    #NdTransformGeo.h \
    #NdExtract2dImageFrom3dImage.h \
    ArrayProperty.h \
    TransFunc.h \
    TransFuncIntensity.h \
    TransFuncMappingKey.h

HEADERS +=\
    GL/GLIndexBufferObject.h \
    GL/GLVertexBufferObject.h \
    GL/GLMesh.h \
    GL/GeomGLTypeDefFwd.h \
    GL/GLPoints.h \
    GL/GLLines.h \
    GL/GLLocation.h \
    GL/GLInstance.h

SOURCES += \
    GL/GLIndexBufferObject.cpp \
    GL/GLVertexBufferObject.cpp \
    GL/GLMesh.cpp \
    GL/GLPoints.cpp \
    GL/GLLines.cpp \
    GL/GLInstance.cpp
