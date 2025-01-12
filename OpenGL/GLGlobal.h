#ifndef SWWGLGLOBAL_H
#define SWWGLGLOBAL_H

#include "OpenGL/OpenGLAPI.h"
#include <GL/glew.h>
#include <GL/GL.h>
#include <ImathMatrix.h>
#include <QGLWidget>

class QGLContext;
class QMainWindow;

namespace SwwGeom {
class Bounds;
}


namespace SwwOpenGL{

#define SWW_MAX_POINT_LIGHT_SOURCE 8
#define SWW_MAX_DIRECTIONAL_LIGHT_SOURCE 2
#define SWW_MAX_SPOT_LIGHT_SOURCE 4

struct SWWOPENGLSHARED_EXPORT CameraBlock{
    Imath::M44f viewMatrix;
    Imath::M44f projectionMatrix;
};

struct SWWOPENGLSHARED_EXPORT PointLightSource
{
    Imath::V4f pos = Imath::V4f(0.0,0.0,0.0,1.0);
    Imath::V4f Ld = Imath::V4f(0.0,0.0,0.0,1.0);
    Imath::V4f Ls = Imath::V4f(0.0,0.0,0.0,1.0);
    Imath::V4f La = Imath::V4f(0.0,0.0,0.0,1.0);
};

struct SWWOPENGLSHARED_EXPORT DirectionalLightSource
{
    Imath::V4f dir = Imath::V4f(0.0,0.0,0.0,1.0);// alpha is padding.
    Imath::V4f Ld = Imath::V4f(0.0,0.0,0.0,1.0);
    Imath::V4f Ls = Imath::V4f(0.0,0.0,0.0,1.0);
    Imath::V4f La = Imath::V4f(0.0,0.0,0.0,1.0);
};

struct SWWOPENGLSHARED_EXPORT SpotLightSource
{
    Imath::V3f pos = Imath::V3f(0.0,0.0,0.0);
    float coneAngle = 45;
    Imath::V3f dir = Imath::V3f(0.0,0.0,0.0);
    float pad0 = 0;
    Imath::V4f Ld = Imath::V4f(0.0,0.0,0.0,1.0);
    Imath::V4f Ls = Imath::V4f(0.0,0.0,0.0,1.0);
    Imath::V4f La = Imath::V4f(0.0,0.0,0.0,1.0);
};

struct SWWOPENGLSHARED_EXPORT LightBlock{
    PointLightSource pointLightSources[SWW_MAX_POINT_LIGHT_SOURCE];
    DirectionalLightSource directionalLightSources[SWW_MAX_DIRECTIONAL_LIGHT_SOURCE];
    SpotLightSource spotLightSources[SWW_MAX_SPOT_LIGHT_SOURCE];
    GLuint numPointLight = 0;
    GLuint numDirectionalLight = 0;
    GLuint numSpotLight = 0;
    GLuint pad0 = 0;
};

class SWWOPENGLSHARED_EXPORT GLGlobal
{
public:
    GLGlobal();
    static bool init();
    static bool deinit();
    static QGLContext* getSharedContext();
    static const QGLContext* getSharedConstContext();
    static const QGLWidget *getShareWidget();
    static void makeCurrent();
    static void doneCurrent();
    static void setMainWindow(QMainWindow* mw);
    static QMainWindow *getMainWindow();

    static const char* CAMERA_UBO_NAME;
    static const char* LIGHT_UBO_NAME;
    //static const GLuint CAMERA_UBO_BINDING_POINT;
    //static const GLuint LIGHT_UBO_BINDING_POINT;
    static const GLuint CAMERA_UBO_DATA_SIZE;
    static const GLuint LIGHT_UBO_DATA_SIZE;
    static const char* MODEL_MATRIX_VARIABLE_NAME;
    static const char* VIEW_MATRIX_VARIABLE_NAME;
    static const char* PROJECTION_MATRIX_VARIABLE_NAME;

    static void addBoundsToSelection(SwwGeom::Bounds* bounds){_selectedNodeBounds.push_back(bounds);}
    static void clearSelection(){_selectedNodeBounds.clear();}

private:
    static std::vector<SwwGeom::Bounds*> _selectedNodeBounds;

    static QGLWidget* _shareWidget;
    static QMainWindow* _mainWindow;
    static QGLContext* _oldContext;
};

}

#endif // SWWGLGLOBAL_H
