#include "OpenGL/RenderDevice.h"

namespace SwwOpenGL {
RenderDevice::RenderDevice( Imath::M44f *viewMatrix,
                            Imath::M44f *projectionMatrix,
                            GLint *viewport,
                            GLShaderProgram *shader,
                            GLFrameBufferObject *fbo):
    _viewMatrix(viewMatrix),_projectionMatrix(projectionMatrix),_viewport(viewport),_shader(shader),_fbo(fbo)
{
}

Imath::V2i RenderDevice::viewportSize()const{
    Imath::V2i size(0,0);
    if(_viewport){
        size.x = _viewport[2] - _viewport[0];
        size.y = _viewport[3] - _viewport[1] ;
    }
    return  size;
}

}//ns
