#ifndef SWWGLUTILITY_H
#define SWWGLUTILITY_H

#include <GL/glew.h>
#include <GL/GL.h>

#include "Core/SwwAssert.h"

#include <iostream>
#include <ImathVec.h>

#include "OpenGL/OpenGLAPI.h"

class QGLContext;

namespace SwwOpenGL {
class SWWOPENGLSHARED_EXPORT GLUtility
{
public:
    GLUtility() {}
    static GLint getCurrentShader();
    static void renderQuad();
    static void drawBox(GLfloat size, GLenum type);
    static void drawCross();
    static void drawCameraManipulator();
    static void drawGrid(float size,float div , const Imath::V4f &color);
    static void printBindState();
    static void printCurrentViewport(const std::string &where = "");
    static bool assertViewportSize(int arg0,int arg1,int arg2,int arg3);
    static const QGLContext* getCurrentContext();
};

}//ns

#endif // SWWGLUTILITY_H
