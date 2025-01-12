#ifndef SWW_RENDERDEVICE_H
#define SWW_RENDERDEVICE_H

//#include "OpenGL/GLTypeDefFwd.h"
#include <ImathMatrix.h>
#include <qglobal.h> //for Q_ASSERT

#include "OpenGL/OpenGLAPI.h"

#include "GL/glew.h"
#include "GL/GL.h"

#include "Core/SwwMacro.h"

namespace SwwOpenGL {

class GLShaderProgram;
class GLFrameBufferObject;

class SWWOPENGLSHARED_EXPORT RenderDevice
{
public:
    RenderDevice(Imath::M44f *viewMatrix,
                 Imath::M44f *projectionMatrix,
                 GLint *viewport,
                 GLShaderProgram *shader,
                 GLFrameBufferObject *fbo);

    const Imath::M44f* viewMatrix()const{
        return _viewMatrix;
    }

    const Imath::M44f* projectionMatrix()const{
        return _projectionMatrix;
    }

    void viewport(const GLint* viewport){_viewport = viewport;}
    const GLint*  viewport()const {return _viewport;}
    void shader(const GLShaderProgram* shader){_shader = shader;}
    const GLShaderProgram* shader()const{return _shader;}

    Imath::V2i viewportSize()const;
    void fbo(const GLFrameBufferObject* fbo){_fbo = fbo;}
    const GLFrameBufferObject* fbo()const{return _fbo;}
    //void restore();

private:
    Imath::M44f* _viewMatrix;
    Imath::M44f* _projectionMatrix;
    const GLint* _viewport;
    const GLShaderProgram*  _shader;
    const GLFrameBufferObject*  _fbo;
};

}

#endif // SWW_RENDERDEVICE_H
