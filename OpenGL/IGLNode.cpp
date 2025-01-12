#include "IGLNode.h"

#include "Core/PlugType.h"
#include <GL/glew.h>
#include <GL/GL.h>
#include "OpenGL/SwwGLMacro.h"

namespace  SwwOpenGL {

const int IGLNode::_stateStackLimit = 32;

bool IGLNode::initialize()
{
    _iTransform = new Sww::Core::M44fPlug("iTransform");
    _iTransform->setConnectable(true);
    _iTransform->setHidden(true);

    addPlug(_iTransform);

    return true;
}

bool IGLNode::executeGL(const Sww::Core::PlugBase *plug,const SwwOpenGL::RenderDevice& device)
{
    return true;
}

void IGLNode::computeBounds(const Sww::Core::V3fArrayConstPtr& posArray){
    _bbox.makeEmpty();
    if ( posArray )
    {
        size_t numPoints = posArray->size();
        for ( size_t p = 0; p < numPoints; ++p )
        {
            const Imath::V3f &P = (*posArray)[p];
            _bbox.extendBy( Imath::V3f( P.x, P.y, P.z ) );
        }
    }
}

void IGLNode::storeGLState(){
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glGetIntegerv(GL_CURRENT_PROGRAM,&_currentProgramId);
    glGetIntegerv(GL_VIEWPORT, _viewport);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &_currentFboId);
    _stateStackCount++;
    Q_ASSERT_X(_stateStackCount<_stateStackLimit,getName().c_str(),"state stack may be over flow.");
}

void IGLNode::restoreGLState(){
    glUseProgram(_currentProgramId);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _currentFboId);
    glViewport(_viewport[0],_viewport[1],_viewport[2],_viewport[3]);
    glPopAttrib();
    _stateStackCount--;
    Q_ASSERT_X(_stateStackCount>=0,getName().c_str(),"state stack may be under flow.");
}


}//ns
