#ifndef SWW_GLNODE_H
#define SWW_GLNODE_H

#include "Core/Node.h"
#include "OpenGL/OpenGLAPI.h"
#include "OpenGL/RenderDevice.h"
#include <ImathBox.h>

namespace Sww::Core{
class M44fPlug;
}

namespace SwwOpenGL {
//class IGLNodePlug;

class SWWOPENGLSHARED_EXPORT IGLNode :public Sww::Core::Node
{
public:
    IGLNode():_stateStackCount(0)
    {};
    virtual ~IGLNode(){}
    virtual bool initialize();
    virtual bool executeGL(const Sww::Core::PlugBase *plug,const SwwOpenGL::RenderDevice& device);

    virtual Imath::Box3f getBBox(){return _bbox;}
    virtual void setBbox(const Imath::Box3f& b){_bbox = b;}

    void storeGLState();
    void restoreGLState();

    Sww::Core::M44fPlug* getITransformPlug()const {return _iTransform;}

protected:
    void computeBounds(const Sww::Core::V3fArrayConstPtr &posArray);

    Imath::Box3f _bbox;
//    int _currentFboId;

    Sww::Core::M44fPlug* _iTransform;

    const static int _stateStackLimit;
private:
    GLint _viewport[4];
    GLint _currentProgramId;
    GLint _currentFboId;
    int _stateStackCount;

    //オーバーライド禁止
    virtual bool execute(const Sww::Core::PlugBase *plug) final {
        return executeGL(plug,SwwOpenGL::RenderDevice(NULL,NULL,NULL,NULL,NULL));
    }

};

}//ns

#endif // SWW_GLNODE_H
