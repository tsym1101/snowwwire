#include "GLGlobal.h"
#include "Core/SwwAssert.h"

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QMainWindow>
#else
#include <QtWidgets/QMainWindow>
#endif

namespace SwwOpenGL {

QGLWidget* GLGlobal::_shareWidget = 0;
QMainWindow* GLGlobal::_mainWindow = 0;
QGLContext* GLGlobal::_oldContext = 0;

const char* GLGlobal::CAMERA_UBO_NAME = "CameraBlock";
const char* GLGlobal::LIGHT_UBO_NAME = "LightBlock";
//const GLuint GLGlobal::CAMERA_UBO_BINDING_POINT = 1;
//const GLuint GLGlobal::LIGHT_UBO_BINDING_POINT = 2;
const GLuint GLGlobal::CAMERA_UBO_DATA_SIZE = sizeof(CameraBlock);
const GLuint GLGlobal::LIGHT_UBO_DATA_SIZE = sizeof(LightBlock);

const char* GLGlobal::MODEL_MATRIX_VARIABLE_NAME = "modelMatrix";
const char* GLGlobal::VIEW_MATRIX_VARIABLE_NAME = "viewMatrix";
const char* GLGlobal::PROJECTION_MATRIX_VARIABLE_NAME = "projectionMatrix";

std::vector<SwwGeom::Bounds*> GLGlobal::_selectedNodeBounds;

GLGlobal::GLGlobal()
{
}

bool GLGlobal::init()
{
    _shareWidget = new QGLWidget();
    _shareWidget->makeCurrent();
    GLenum err = glewInit();
    if (err != GLEW_OK){
        SWW_ERR("glewInit failed. " << glewGetErrorString(err));
    }

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    _shareWidget->doneCurrent();

    SWW_DEBUG("openGL init success.");
    return true;
}

bool GLGlobal::deinit()
{
    Q_ASSERT(_shareWidget);
    delete _shareWidget;
    return true;
}

QGLContext* GLGlobal::getSharedContext()
{
    Q_ASSERT(_shareWidget);
    return  const_cast<QGLContext*>(_shareWidget->context());
}

const QGLContext* GLGlobal::getSharedConstContext()
{
    Q_ASSERT(_shareWidget);
    return  _shareWidget->context();
}

const QGLWidget* GLGlobal::getShareWidget()
{
    return _shareWidget;
}

void GLGlobal::makeCurrent()
{
    Q_ASSERT(_shareWidget);
    _oldContext = const_cast<QGLContext*>(QGLContext::currentContext());
    _shareWidget->makeCurrent();
}

void GLGlobal::doneCurrent()
{
    if(_oldContext)_oldContext->makeCurrent();
    else _shareWidget->doneCurrent();
}

void GLGlobal::setMainWindow(QMainWindow* mw)
{
    _mainWindow = mw;
}

QMainWindow *GLGlobal::getMainWindow()
{
    return _mainWindow;
}


}//ns
