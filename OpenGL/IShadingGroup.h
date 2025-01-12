#ifndef SWW_SHADINGGROUP_H
#define SWW_SHADINGGROUP_H

#include "OpenGL/IGLNode.h"
#include "OpenGL/RenderDevice.h"
#include "OpenGL/GLTypeDefFwd.h"



namespace SwwGeom {
class ShapePlug;
class MeshPlug;
}

namespace SwwOpenGL {

class IShadingGroupPlug;

class  SWWOPENGLSHARED_EXPORT IShadingGroup :public Sww::Core::Node
{
public:
    IShadingGroup();
    virtual ~IShadingGroup(){}
    virtual bool initialize();
    virtual bool setUniform() = 0;

    const SwwOpenGL::GLShaderProgramPtr& getShader()const{return _shader;}
    template <class DrawOperator> bool drawShapes(DrawOperator& f);

    bool bindUbo(const std::vector<SwwOpenGL::GLUniformBufferObject*>& uboVec);

protected:
     SwwOpenGL::GLShaderProgramPtr _shader;
     SwwGeom::ShapePlug* _iShapePlug;
     IShadingGroupPlug* _shadingGroupPlug;
     std::vector<SwwOpenGL::GLUniformBufferObject*> _uboVec;//bindされたuboはとっておく

     bool restoreUbo()const;
};

template <class DrawOperator>
inline bool IShadingGroup::drawShapes(DrawOperator& f){
    return _iShapePlug->walk(f);
}

}//ns SwwOpenGL

#endif // SWW_SHADINGGROUP_H
