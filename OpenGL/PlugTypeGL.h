#ifndef PLUGTYPEGL_H
#define PLUGTYPEGL_H

#include "Core/PlugType.h"
#include "OpenGL/GLTypeDefFwd.h"

#include "OpenGL/RenderDevice.h"
#include "OpenGL/OpenGLAPI.h"

namespace SwwOpenGL {

class IShadingGroup;
class IRenderer;

enum SwwTypeDescGL
{
    kGLNODE = Sww::Core::OPENGL_TYPE + 0,
    //kGLCAMERA = Sww::Core::OPENGL_TYPE + 1,
    kGLTEXTURE = Sww::Core::OPENGL_TYPE + 2,
    //kGLRENDERER = Sww::Core::OPENGL_TYPE + 3,
    kGLLIGHT = Sww::Core::OPENGL_TYPE + 4,
    //kGLRENDERTARGET = Sww::Core::OPENGL_TYPE + 5,
    //kGLOBJECT = Sww::Core::OPENGL_TYPE + 6,
    kGLTF= Sww::Core::OPENGL_TYPE + 7,
    //kGLSHADINGGROUP= Sww::Core::OPENGL_TYPE + 8,
    kRENDERER = Sww::Core::OPENGL_TYPE + 9,
    kSHADINGGROUP = Sww::Core::OPENGL_TYPE + 10,
    kTRANSFERFUNCTION= Sww::Core::OPENGL_TYPE + 11,
    kGL4TYPE = Sww::Core::OPENGL_TYPE + 50
};

class IGLNode;
class NdGLCamera;
class NdGLLight;

class SWWOPENGLSHARED_EXPORT IGLNodePlug : public Sww::Core::Plug<IGLNode*>
{


public :
    IGLNodePlug(const std::string& name = "noName",IGLNode* data = 0);
    virtual SwwTypeDesc getSwwTypeDesc()const;
    virtual std::string getSwwTypeDescString()const;
    virtual Sww::Core::PlugColor getPlugColor()const;

    bool IGLNodePlug::renderGL(const SwwOpenGL::RenderDevice& device);

private:
    typedef std::vector<Sww::Core::PlugBase*> PlugBaseArray;

    bool pullGL(const SwwOpenGL::RenderDevice& device);
    void getConstInputValue(bool *ok){}
    std::vector<IGLNode*> _children;

public:
    DECLARE_PLUG_CREATOR(IGLNodePlug)
};

class  SWWOPENGLSHARED_EXPORT GLTexturePlug : public Sww::Core::Plug<SwwOpenGL::GLTexturePtr>
{
public :
    GLTexturePlug(const std::string& name,SwwOpenGL::GLTexturePtr data = SwwOpenGL::GLTexturePtr());
    virtual SwwTypeDesc getSwwTypeDesc()const;
    virtual std::string getSwwTypeDescString()const;
    virtual Sww::Core::PlugColor getPlugColor()const;
    virtual const SwwOpenGL::GLTexturePtr& getTexture(bool* ok);

private:
    inline const SwwOpenGL::GLTexturePtr& getConstInputValue(bool* ok){
        return Sww::Core::Plug<SwwOpenGL::GLTexturePtr>::getConstInputValue(ok);
    }

public:
    DECLARE_PLUG_CREATOR(GLTexturePlug)
};


class SWWOPENGLSHARED_EXPORT IRendererPlug : public Sww::Core::Plug<IRenderer*>
{
public :
    IRendererPlug(const std::string& name,IRenderer* data = 0);
    virtual SwwTypeDesc getSwwTypeDesc()const;
    virtual std::string getSwwTypeDescString()const;
    virtual Sww::Core::PlugColor getPlugColor()const;

    DECLARE_PLUG_CREATOR(IRendererPlug)
};

class SWWOPENGLSHARED_EXPORT IShadingGroupPlug : public Sww::Core::Plug<IShadingGroup*>
{
public :
    IShadingGroupPlug(const std::string& name,IShadingGroup* data = 0);
    virtual SwwTypeDesc getSwwTypeDesc()const;
    virtual std::string getSwwTypeDescString()const;
    virtual Sww::Core::PlugColor getPlugColor()const;

    DECLARE_PLUG_CREATOR(IShadingGroupPlug)

};

}//ns

#endif // PLUGTYPEGL_H


