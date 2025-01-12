#-------------------------------------------------
#
# Project created by QtCreator 2015-09-17T17:19:06
#
#-------------------------------------------------

# Check if the config file exists
! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

include(../external/glCommon.pri)
#include(../external/openimageioCommon.pri)
#include(../external/openCvCommon.pri)

CONFIG(debug, debug|release) {
   DEFINES += QT_DEBUG #念のため
}

DEPENDPATH += $$PWD/../Core/\
              $$PWD/../Util/

QT       += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SwwOpenGL
TEMPLATE = lib

DEFINES += SWWOPENGL_LIBRARY

LIBS += -L$${DESTDIR} -lSwwUtil -lSwwCore -lSwwGeom -lSwwKnob

##poly2tri*************************************************
#INCLUDEPATH +=  $${SWW_LIBS_ROOT}/poly2tri/include

#CONFIG(debug, debug|release) {
#        LIBS +=\
#        $${SWW_LIBS_ROOT}/poly2tri/lib/poly2trid.lib
#}
#CONFIG(release, debug|release) {
#        LIBS +=\
#        $${SWW_LIBS_ROOT}/poly2tri/lib/poly2tri.lib
#}
##**********************************************************



SOURCES += \
    GLFrameBufferObject.cpp \
    GLGlobal.cpp \
    GLImageBufferObject.cpp \
    GLLegacyVbo.cpp \
    GLRenderTarget.cpp \
    GLShaderProgram.cpp \
    GLTexture.cpp \
    GLUniformBufferObject.cpp \
    GLUniformBufferObjectHelper.cpp \
    GLUtility.cpp \
    RenderDevice.cpp \
    IGLNode.cpp \
    IGLObject.cpp \
    IGLUniform.cpp \
    IRenderer.cpp \
    IShadingGroup.cpp \
    IXForm.cpp \
    PlugTypeGL.cpp \
    UniformPlugFactory.cpp \
    GLShaderHelper.cpp \

HEADERS +=\
    GLDrawRoutine.h \
    GLFrameBufferObject.h \
    GLGlobal.h \
    GLImageBufferObject.h \
    GLLegacyVbo.h \
    GLRenderTarget.h \
    GLShaderProgram.h \
    GLTexture.h \
    GLTypeDefFwd.h \
    GLUniformBufferObject.h \
    GLUniformBufferObjectHelper.h \
    GLUtility.h \
    MathDef.h \
    RenderDevice.h \
    SwwGLMacro.h \
    IGLNode.h \
    IGLObject.h \
    IGLUniform.h \
    IRenderer.h \
    IShadingGroup.h \
    IXForm.h \
    OpenGLAPI.h \
    GLDrawRoutine.h \
    MathDef.h \
    PlugTypeGL.h \
    GLTypeDefFwd.h \
    UniformPlugFactory.h \
    GLShaderHelper.h \
    SwwGLMacro.h \
