#include "IRenderer.h"
#include "OpenGL/PlugTypeGL.h"

#include <glm/gtc/matrix_transform.hpp>

namespace  SwwOpenGL{

const int IRenderer::_stateStackLimit = 256;

IRenderer::IRenderer():_stateStackCount(0)
{
}

bool IRenderer::initialize()
{
    _camera = new SwwGeom::CameraPlug("camera");
    _camera->setConnectable(true);

    _renderer = new SwwOpenGL::IRendererPlug("renderer");
    _renderer->setConnectable(true);
    _renderer->setOutput(true);

    addPlug(_camera);
    addPlug(_renderer);

    _camera->affects(_renderer);

    _renderer->setOutputValue(this);

    return true;
}

//bool IRenderer::execute(const Sww::Core::PlugBase *plug)
//{
//    //SWW_JUST_UPDATE(_camera);
//    return true;
//}


QString IRenderer::toolTip()const
{
    QString str = "your tooltip";
    return str;
}

const SwwGeom::Camera* IRenderer::getCamera()const{
    SWW_VAL(_camera,cam);
    return cam;
}

void IRenderer::storeGLState(){
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glGetIntegerv(GL_CURRENT_PROGRAM,&_currentProgramId);
    glGetIntegerv(GL_VIEWPORT, _viewport);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &_currentFboId);
    _stateStackCount++;
    Q_ASSERT_X(_stateStackCount<_stateStackLimit,getName().c_str(),"state stack may be over flow.");
}

void IRenderer::restoreGLState(){
    glUseProgram(_currentProgramId);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _currentFboId);
    glViewport(_viewport[0],_viewport[1],_viewport[2],_viewport[3]);
    glPopAttrib();
    _stateStackCount--;
    Q_ASSERT_X(_stateStackCount>=0,getName().c_str(),"state stack may be under flow.");
}

bool IRenderer::render(const SwwOpenGL::RenderDevice &device){
    QMutexLocker locker(mutex());
    if(!doRender(device)){
        setErrorOccurred(true);
        return false;
    }
    setErrorOccurred(false);
    return true;
}

Imath::M44f IRenderer::defaultProjectionMatrix(){
    glm::mat4 projTmp = glm::ortho(-1.0,1.0,-1.0,1.0,0.01,2.0);
    Imath::M44f projectionMatrix = *((Imath::M44f*)&projTmp);
    return projectionMatrix;
}

Imath::M44f IRenderer::defaultViewMatrix(){
    Imath::M44f m;
    m.translate(Imath::V3f(0.0,0.0,-1.0));
    return m;
}

}//ns SwwOpenGL

