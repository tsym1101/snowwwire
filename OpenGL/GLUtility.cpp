#include "OpenGL/GLUtility.h"

#include "Core/SwwAssert.h"

#include <GL/glew.h>
#include <QGLContext>

namespace SwwOpenGL {

GLint GLUtility::getCurrentShader()
{
    GLint id;
    glGetIntegerv(GL_CURRENT_PROGRAM,(GLint*)&id);
    return id;
}

void GLUtility::renderQuad()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    const double upper =1.0;
    const double lower =-1.0;

    glBegin(GL_QUADS);
    {
        glTexCoord2d(0.0, 0.0);
        glVertex2f(lower, lower);
        glTexCoord2d(1.0, 0.0);
        glVertex2f(upper, lower);
        glTexCoord2d(1.0, 1.0);
        glVertex2f(upper, upper);
        glTexCoord2d(0.0, 1.0);
        glVertex2f(lower, upper);
    }

    glEnd();
    glPopMatrix();

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();
}

void GLUtility::drawBox(GLfloat size, GLenum type)
{
    static GLfloat n[6][3] =
    {
        {-1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] =
    {
        {0, 1, 2, 3},
        {3, 2, 6, 7},
        {7, 6, 5, 4},
        {4, 5, 1, 0},
        {5, 6, 2, 1},
        {7, 4, 0, 3}
    };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    for (i = 5; i >= 0; i--) {
        glBegin(type);
        glNormal3fv(&n[i][0]);
        glVertex3fv(&v[faces[i][0]][0]);
        glVertex3fv(&v[faces[i][1]][0]);
        glVertex3fv(&v[faces[i][2]][0]);
        glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }
}

void GLUtility::drawCross()
{
    glPushMatrix();
    glColor3f(0.7f,0.7f,0.7f);
    glBegin(GL_LINES);
    glVertex3f(-1,0,0);
    glVertex3f(1,0,0);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0,-1,0);
    glVertex3f(0,1,0);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0,0,-1);
    glVertex3f(0,0,1);
    glEnd();
    glPopMatrix();
}

void GLUtility::drawCameraManipulator()
{
    glPushMatrix();
    glColor3f(0.7f,0.7f,0.7f);
    drawBox(0.25,GL_LINE_LOOP);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.7f,0.7f,0.7f);
    glTranslatef(0,0,0.5);
    glScalef(0.5,0.5,1.0);
    drawBox(1,GL_LINE_LOOP);
    glPopMatrix();
}

void GLUtility::drawGrid(float size,float div , const Imath::V4f &color)
{
    //    glBegin(GL_LINES);
    //    glColor4f (color.x,color.y,color.z,color.w);
    //    glTexCoord2d(0.0, 1.0);
    //    glVertex3f(-1,0, -1);
    //    glTexCoord2d(1.0, 1.0);
    //    glVertex3f(-1,0 ,1);
    //    glTexCoord2d(1.0, 0.0);
    //    glVertex3f(1,0,1);
    //    glTexCoord2d(0.0, 0.0);
    //    glVertex3f(1,0, -1);
    //    glEnd();

    glBegin(GL_LINES);
    glColor4f (color.x,color.y,color.z,color.w);

    float edge = size / 2.f;
    float xStep = size / div;
    for(float i=-edge;i<=edge;i+=xStep) {
        glVertex3f(i ,0,-edge);
        glVertex3f(i ,0,edge);

        glVertex3f(edge,0,i);
        glVertex3f(-edge,0,i);
    }
    glEnd();
}

void GLUtility::printBindState()
{
    int currentFboId;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentFboId);
    int currentProgramId;
    glGetIntegerv(GL_CURRENT_PROGRAM,(GLint*)&currentProgramId);

    SWW_DEBUG("current context:" << QGLContext::currentContext() << " fbo:" << currentFboId << " programId:" << currentProgramId);
}

void GLUtility::printCurrentViewport(const std::string& where)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    SWW_DEBUG("current viewport at "  << where << ":" << viewport[0] << ","
                                                << viewport[1] << ","
                                                << viewport[2] << ","
                                                << viewport[3]);
}

bool GLUtility::assertViewportSize(int arg0, int arg1, int arg2, int arg3)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    return (viewport[0] == arg0 &&
            viewport[1] == arg1 &&
            viewport[2] == arg2 &&
            viewport[3] == arg3);
}

const QGLContext* GLUtility::getCurrentContext()
{
    return QGLContext::currentContext();
}

}//ns
