#ifndef SWWGLMACRO_H
#define SWWGLMACRO_H

#include <GL/glew.h>
#include <GL/GL.h>
#include <QGLContext>
#include "Core/SwwAssert.h"

#ifdef QT_DEBUG
#define CHECK_GL_ERRORS  \
{ \
    GLenum err = glGetError(); \
    if (err) {\
    if(err == GL_INVALID_ENUM) {SWW_ERR("GL err :" << err << " invalid enum.");}\
    else if(err == GL_INVALID_VALUE) {SWW_ERR("GL err :" << err << " invalid value.");}\
    else if(err == GL_INVALID_OPERATION) {SWW_ERR("GL err :" << err << " invalid operation.");}\
    else if(err == GL_STACK_OVERFLOW) {SWW_ERR("GL err :" << err << " stack over flow.");}\
    else if(err == GL_STACK_UNDERFLOW) {SWW_ERR("GL err :" << err << " stack under flow.");}\
    else if(err == GL_OUT_OF_MEMORY) {SWW_ERR("GL err :" << err << " GL out of memoty. memory shortage.");}\
    else if(err == GL_INVALID_FRAMEBUFFER_OPERATION) {SWW_ERR("GL err :" << err << " invalid frame buffer.");}\
    else {SWW_ERR("GL err :" << err << " unknown err.");}\
    }\
}
#else
#define CHECK_GL_ERRORS {}
#endif

#ifdef QT_DEBUG
#define SWW_CHECK_GL_CONTEXT   \
    Q_ASSERT_X(QGLContext::currentContext(),\
               "check gl context.",\
               "no context is current.");
#else
#define SWW_CHECK_GL_CONTEXT {}
#endif



#ifdef QT_DEBUG
#define SWW_CHECK_GL_SHADER_CURRENT   \
    GLint id;\
    glGetIntegerv(GL_CURRENT_PROGRAM,(GLint*)&id);\
    Q_ASSERT_X(id,\
               "check gl shader current.",\
               "no shader is current.");
#else
#define SWW_CHECK_GL_CONTEXT {}
#define SWW_CHECK_GL_SHADER_CURRENT  {}
#endif

#endif // SWWGLMACRO_H
