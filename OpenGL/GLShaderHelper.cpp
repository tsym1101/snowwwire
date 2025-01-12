#include "GLShaderHelper.h"
#include "OpenGL/GLShaderProgram.h"
#include "OpenGL/UniformPlugFactory.h"
#include "Core/Node.h"
#include "OpenGL/SwwGLMacro.h"

#include "OpenGL/GLUniformBufferObjectHelper.h"
#include "OpenGL/IGLUniform.h"


namespace SwwOpenGL {

GLShaderHelper::GLShaderHelper(SwwOpenGL::GLShaderProgramPtr shader,
                               Sww::Core::Node *parent):_shader(shader),_parent(parent),_ignoreUbo(false),_isInitilized(false)
{
    Q_ASSERT_X(_parent,"GLShaderHelper","parent node is invalid.");
}

GLShaderHelper::~GLShaderHelper(){


}

void GLShaderHelper::init(){
    Q_ASSERT_X(!_isInitilized,_parent->getName().c_str(),"init() is called twice.");

    SWW_CHECK_GL_CONTEXT;
    glValidateProgram(_shader->programId());
    GLenum err = glGetError();
    Q_ASSERT_X(!err,_parent->getName().c_str(),"invalid shader.");

    _parent->storeDynamicPlugValueState();
    _parent->storeDynamicPlugConnectState();
    createUniformPlug();
    _parent->restoreDynamicPlugConnectState();
    _parent->restoreDynamicPlugValueState();

    _isInitilized = true;
}

void GLShaderHelper::createUniformPlug()
{
    SWW_CHECK_GL_CONTEXT;

    _parent->destroyDynamicPlugItemArray();//初期化

    Q_ASSERT(_shader);
    Q_ASSERT(_shader->isLinked());

    int total = -1;
    glGetProgramiv(_shader->programId(), GL_ACTIVE_UNIFORMS, &total );

    //組み込みはスキップ
    _ignoreList.push_back("gl_ModelViewMatrix");
    _ignoreList.push_back("gl_ProjectionMatrix");
    _ignoreList.push_back("gl_ModelViewProjectionMatrix");

    if(!_ignoreUbo){
        SwwOpenGL::UboMetaArray uboMetaList;
        SwwOpenGL::GLUniformBufferObjectHelper::getUboMeta(_shader,uboMetaList);
        addUniformToIgnoreList(uboMetaList);
    }

    UniformPlugFactory factory;

    for(int i=0; i < total; ++i){
        int name_len=-1, num=-1;
        GLenum type = GL_ZERO;
        char name[256];
        glGetActiveUniform(_shader->programId(), GLuint(i), sizeof(name)-1,&name_len, &num, &type, name );
        name[name_len] = 0;


        //組み込みはスキップ
        std::string nameTmp(name) ;
        if(vectorContains(_ignoreList,nameTmp) )continue;

        //新規生成
        Sww::Core::PlugBase* plug = factory.generate(_shader,name,type);
        Q_ASSERT_X(plug,plug->getName().c_str(),"null");
        plug->setConnectable(true);
        plug->setStorable(true); // 保存できるものは保存してしまう

#if 0
        if(_parent->isInitilized())_parent->addDynamicPlug(plug);
        else _parent->addPlug(plug);
#else
        _parent->addDynamicPlug(plug);
#endif

        SWW_VECTOR_FOREACH(Sww::Core::PlugBase*,_affectedPlugs){
            Sww::Core::PlugBase* p = *i;
            plug->affects(p);
        }

        IGLUniform* u = dynamic_cast<IGLUniform*>(plug);//多重継承した親同士のキャスト
        Q_ASSERT_X(u,plug->getName().c_str(),"dynamic cast failed.");

        _generatedUniforms.push_back(u);

    }//for

    _ignoreList.clear(); //最後にクリアしないとaddToIgnoreListで追加した分が動作しない
}

bool GLShaderHelper::justUpdate(){
    Q_ASSERT_X(_isInitilized,_parent->getName().c_str(),"GLShaderHelper is not initilized.");

    SWW_VECTOR_FOREACH(IGLUniform*,_generatedUniforms){
        IGLUniform* p = *i;
        SWW_CHECK_BOOL(p->justUpdate());
    }
    return true;
}

bool GLShaderHelper::upload(){
    Q_ASSERT_X(_isInitilized,_parent->getName().c_str(),"GLShaderHelper is not initilized.");

    //SWW_VECTOR_FOREACH(Sww::Core::PlugBase*,_generatedPlugs){
        //Sww::Core::PlugBase* p = *i;
    SWW_VECTOR_FOREACH(IGLUniform*,_generatedUniforms){
        IGLUniform* p = *i;
//        if(p->getUniformName() == "prefilterMap"){
//            SWW_DEBUG("prefilterMap......");
//        }
       // SWW_DEBUG_VALUE(p->getUniformName());
        SWW_CHECK_BOOL(p->setUniform());
        //CHECK_GL_ERRORS;
    }
    return true;
}

void GLShaderHelper:: addUniformToIgnoreList(const SwwOpenGL::UboMetaArray &uboMetaList){

    for(std::vector<SwwOpenGL::UboMeta>::const_iterator it = uboMetaList.begin();it != uboMetaList.end();++it){
        const SwwOpenGL::UboMeta& m = *it;
        for(std::vector<SwwOpenGL::UniformMetaInUbo>::const_iterator i = m._uniformMetaList.begin();i != m._uniformMetaList.end();++i){
            const SwwOpenGL::UniformMetaInUbo& metaInUbo = *i;
            _ignoreList.push_back(metaInUbo._name);
        }
    }
}


void GLShaderHelper::addToIgnoreList(const std::string &name){

    Q_ASSERT_X(!_isInitilized,_parent->getName().c_str(),"addToIgnoreList() call after init().");
    _ignoreList.push_back(name);
}

void GLShaderHelper::affects(Sww::Core::PlugBase* p){
    Q_ASSERT_X(!_isInitilized,_parent->getName().c_str(),"affects() call after init().");
    _affectedPlugs.push_back(p);
}

}//ns
