#ifndef GLSHADERHELPER_H
#define GLSHADERHELPER_H

#include "OpenGL/PlugTypeGL.h"
//#include "Core/SwwAssert.h"
#include "Core/SwwMacro.h"
#include "Geom/GeomAPI.h"
#include "OpenGL/GLTypeDefFwd.h"


namespace Sww::Core{
class Node;
}
namespace SwwOpenGL {

class IGLUniform;


class SWWOPENGLSHARED_EXPORT GLShaderHelper
{

public:
    GLShaderHelper(SwwOpenGL::GLShaderProgramPtr shader,Sww::Core::Node *parent);
    ~GLShaderHelper();

    //typedef std::vector<IGLUniform*> GLUniformArray;

    bool justUpdate();
    bool upload();
    bool valid(){return _shader.get();}

    void setIgnoreUbo(bool b){_ignoreUbo = b;}
    void addToIgnoreList(const std::string& name);
    void affects(Sww::Core::PlugBase* p);

    void init();

    const SwwOpenGL::GLUniformArray& getUniformArray()const{return _generatedUniforms;}

private:
    typedef std::vector<Sww::Core::PlugBase*> AffectedPlugVec;

    SwwOpenGL::GLShaderProgramPtr _shader;
    Sww::Core::Node *_parent;

    //std::vector<Sww::Core::PlugBase*> _generatedPlugs;
    SwwOpenGL::GLUniformArray _generatedUniforms;

    void addUniformToIgnoreList(const SwwOpenGL::UboMetaArray& uboMetaList);
    //↓initialize()の中で呼び出してはならない
    void createUniformPlug();

    bool _ignoreUbo;
    Sww::Core::StringArray _ignoreList;
    AffectedPlugVec _affectedPlugs;

    bool _isInitilized;

    //このクラスはコピー禁止にしておく
    GLShaderHelper(const  GLShaderHelper&);            // コピーコンストラクタ
    GLShaderHelper operator=(const  GLShaderHelper&);  // 代入演算子オーバーロード
};

}//ns

#endif // GLSHADERHELPER_H
