#include "OpenGL/PlugTypeGL.h"
#include "OpenGL/IGLNode.h"
//#include "OpenGL/TransFuncIntensity.h"
#include "OpenGL/SwwGLMacro.h"
#include "OpenGL/GLTexture.h"

namespace SwwOpenGL {

IGLNodePlug::IGLNodePlug(const std::string& name,IGLNode* data):
    Sww::Core::Plug<IGLNode*>(name,data){
    setHidden(true);}
SwwTypeDesc IGLNodePlug::getSwwTypeDesc()const{return SwwOpenGL::kGLNODE;}
std::string IGLNodePlug::getSwwTypeDescString()const{return "glNode";}
Sww::Core::PlugColor IGLNodePlug::getPlugColor()const{return Sww::Core::PlugColor(180,255,0);}

bool IGLNodePlug::pullGL(const SwwOpenGL::RenderDevice &device){
    Q_ASSERT_X(0,"call pullGL","discontinued");

//    Q_ASSERT_X(isOutput(),getDebugName().c_str(),"pullGL in input plug.");

//    if(connectedPlugsNegative().size() == 0){
//        IGLNode* glNode = rawValue();
//        QMutexLocker locker(glNode->mutex());//mutex lock.
//        if(!glNode->executeGL(this,device)){
//            SWW_ERR("executeGL failed. " << getDebugName());
//            glNode->setErrorOccurred(true);
//            return false;
//        }
//        glNode->setErrorOccurred(false);

//    }else{
//        for(PlugBaseArray::iterator it = connectedPlugsNegative().begin();
//            it != connectedPlugsNegative().end() ; ++it){
//            Sww::Core::PlugBase* p0 =  *it;
//            IGLNodePlug* glNodePlug = static_cast<IGLNodePlug*>(p0);

//            Q_ASSERT_X(dynamic_cast<IGLNodePlug*>(p0),getDebugName().c_str(),getDebugName().c_str());
//            Q_ASSERT_X(glNodePlug,getDebugName().c_str(),"invalid glNodePlug.");
//            Q_ASSERT_X(glNodePlug->rawValue(),glNodePlug->getDebugName().c_str(),"has no IGLNode.");

//            if(!glNodePlug->pullGL(device)){return false;}
//        }//for
//    }

//    setClean();//これを忘れるとpushDirty()が途中で止まる

    return true;
}

bool IGLNodePlug::renderGL(const SwwOpenGL::RenderDevice& device)
{
    Q_ASSERT_X(0,"call renderGL","discontinued");
//    Q_ASSERT_X(!isOutput(),getDebugName().c_str(),"render in output plug.");

//    for(PlugBaseArray::iterator i = _connectedPlugs.begin();
//        i != _connectedPlugs.end() ; ++i){

//        Sww::Core::PlugBase* p =  *i;
//        IGLNodePlug* glNodePlug = static_cast<IGLNodePlug*>(p);

//        Q_ASSERT_X(dynamic_cast<IGLNodePlug*>(p),getDebugName().c_str(),p->getDebugName().c_str());
//        Q_ASSERT_X(dynamic_cast<IGLNode*>(p->getNode()),getDebugName().c_str(),"node is not IGLNode.");
//        Q_ASSERT_X(glNodePlug->rawValue(),getDebugName().c_str(),"has no IGLNode.");

//        if(!glNodePlug->pullGL(device)){return false;}

//    }
//    setClean();

    return true;

}

GLTexturePlug::GLTexturePlug(const std::string& name, SwwOpenGL::GLTexturePtr data )
    :Sww::Core::Plug<SwwOpenGL::GLTexturePtr>(name,data){
    setHidden(true);
}
SwwTypeDesc  GLTexturePlug::getSwwTypeDesc()const{return SwwOpenGL::kGLTEXTURE;}
std::string  GLTexturePlug::getSwwTypeDescString()const{return "glTexture";}
Sww::Core::PlugColor  GLTexturePlug::getPlugColor()const{return Sww::Core::PlugColor(20,160,100);}

const SwwOpenGL::GLTexturePtr& GLTexturePlug::getTexture(bool* ok){
    GLint current;
    glGetIntegerv(GL_CURRENT_PROGRAM,&current);
    glUseProgram(0);
    CHECK_GL_ERRORS;
    const SwwOpenGL::GLTexturePtr& data = Sww::Core::Plug<SwwOpenGL::GLTexturePtr>::getConstInputValue(ok);
    glUseProgram(current);
    if(!*ok){
        SWW_ERR("get texture failed. " << getDebugName());
        return 0;
    }
    if(!data){
        SWW_ERR("empty texture." << getDebugName());
        *ok = false;
        return 0;
    }
    return data;
}

IRendererPlug::IRendererPlug(const std::string& name,IRenderer* data):Sww::Core::Plug<IRenderer*>(name,data){
    setHidden(true);
}
SwwTypeDesc  IRendererPlug::getSwwTypeDesc()const{return SwwOpenGL::kRENDERER;}
std::string  IRendererPlug::getSwwTypeDescString()const{return "renderer";}
Sww::Core::PlugColor  IRendererPlug::getPlugColor()const{return Sww::Core::PlugColor(70,20,20);}


IShadingGroupPlug::IShadingGroupPlug(const std::string& name,IShadingGroup* data):Sww::Core::Plug<IShadingGroup*>(name,data){
    setHidden(true);
}
SwwTypeDesc  IShadingGroupPlug::getSwwTypeDesc()const{return SwwOpenGL::kSHADINGGROUP;}
std::string  IShadingGroupPlug::getSwwTypeDescString()const{return "shadingGroup";}
Sww::Core::PlugColor  IShadingGroupPlug::getPlugColor()const{return Sww::Core::PlugColor(20,150,150);}

}//ns
