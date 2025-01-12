#ifndef SWW_RENDERER_H
#define SWW_RENDERER_H

#include "Core/Node.h"
//#include "Core/PlugTypeDefFwd.h"

#include "OpenGL/PlugTypeGL.h"
#include "Geom/PlugTypeGeom.h"
#include "OpenGL/RenderDevice.h"

#include <GL/glew.h>
#include <GL/GL.h>

namespace SwwOpenGL {

class IRendererPlug;

class SWWOPENGLSHARED_EXPORT IRenderer :public Sww::Core::Node
{
public:
    IRenderer();
    virtual ~IRenderer(){}
    virtual bool initialize();
    //virtual bool execute(const Sww::Core::PlugBase *plug);
    bool render(const SwwOpenGL::RenderDevice& device);
    virtual QString toolTip()const;

    const SwwGeom::Camera* getCamera()const;

    static Imath::M44f defaultProjectionMatrix();
    static Imath::M44f defaultViewMatrix();

protected:
    virtual bool doRender(const SwwOpenGL::RenderDevice& device) = 0;

    SwwGeom::CameraPlug* _camera;
    IRendererPlug* _renderer;

    void storeGLState();
    void restoreGLState();
    const static int _stateStackLimit;
private:
    GLint _viewport[4];
    GLint _currentProgramId;
    GLint _currentFboId;
    int _stateStackCount;

//    //オーバーライド禁止
//    virtual bool execute(const Sww::Core::PlugBase *plug) final {
//        return executeGL(plug,RenderDevice());
//    }
};

}//ns SwwOpenGL

#endif // SWW_RENDERER_H

