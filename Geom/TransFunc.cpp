#include "TransFunc.h"
namespace SwwGeom {

TransFunc::TransFunc(int width, int height, int depth)
    :dimensions_(width, height, depth)
    //,textureInvalid_(true)
    ,_mustResetTexture(true)
{
}

TransFunc::~TransFunc() {
    //    if(_tex){
    //        delete _tex;
    //        _tex = 0;
    //    }
}
int TransFunc::getNumDimensions() const {
    if(dimensions_.z == 1) {
        if(dimensions_.y == 1)
            return 1;
        else
            return 2;
    }
    else
        return 3;
}

//void TransFunc::bindUniform(GLuint programId,
//                            const std::string &varName,
//                            int texunit){
//    SWW_CHECK_GL_CONTEXT;
//    _tex->bindUniform(programId,varName,texunit);
//}


//void TransFunc::invalidateTexture() {
//    textureInvalid_ = true;
//}
//bool TransFunc::isTextureInvalid() const {
//    return textureInvalid_;
//}

bool TransFunc::mustResetTexture()const{
    return _mustResetTexture;
}

//void TransFunc::bind() {
//    SWW_CHECK_GL_CONTEXT;
//    if (textureInvalid_)
//        updateTexture();

//    _tex->bind();
//}

//void TransFunc::updateTexture() {

//    if (!_tex || (Imath::V3i(_tex->width(),_tex->height(),_tex->depth()) != dimensions_))
//        createTex();

//    if (!_tex) {
//        std::cout << "Failed to create texture" << std::endl;
//        return;
//    }

//    //tex_->uploadTexture();
//    //_tex->setData();
//    _tex->uploadInternalData();
//    textureInvalid_ = false;
//}

//void TransFunc::createTex() {
//    SWW_CHECK_GL_CONTEXT;
//    GLint internalFormat = GL_RGBA8;
//    if(dataType_ == GL_FLOAT)internalFormat = GL_RGBA32F;

//    _tex.reset(new SwwOpenGL::GLTexture(dimensions_.x,
//                                       dimensions_.y,
//                                       dimensions_.z,
//                                       _texTarget,
//                                       internalFormat,
//                                       GL_RGBA,
//                                       dataType_,
//                                       GL_NEAREST,
//                                       GL_CLAMP));

//    _tex->allocateData();
//    //_textureAllocated = true;
//}

//const SwwOpenGL::GLTexturePtr& TransFunc::getTexture() {
//    if (textureInvalid_)
//        updateTexture();

//    return _tex;
//}

std::string TransFunc::getSamplerType() const {
    if (dimensions_.z > 1)
        return "TransFunc3D";
    else if (dimensions_.y > 1)
        return "TransFunc2D";
    else
        return "TransFunc1D";
}

std::string TransFunc::getShaderDefines() const {
    return "#define TF_SAMPLER_TYPE " + getSamplerType() + "\n";
}

void TransFunc::resize(int width, int height, int depth) {

    if (width != dimensions_.x) {
        dimensions_.x = width;
       // textureInvalid_ = true;
        _mustResetTexture = true;
    }
    if (height != dimensions_.y) {
        dimensions_.y = height;
        //textureInvalid_ = true;
        _mustResetTexture = true;
    }
    if (depth != dimensions_.z) {
        dimensions_.z = depth;
        //textureInvalid_ = true;
        _mustResetTexture = true;
    }
}

const Imath::V3i &TransFunc::getDimensions() const {
    return dimensions_;
}

//GLint TransFunc::getFormat() const {
//    return format_;
//}

//GLenum TransFunc::getTexTarget() const {
//    return _texTarget;
//}

//void TransFunc::setTexTarget(GLenum target){
//    Q_ASSERT_X(target == GL_TEXTURE_1D || target == GL_TEXTURE_2D
//               ,"transfunc setTexTarget."
//               ,"invalid target.");
//    _texTarget = target;
//    textureInvalid_ = true;
//    _mustResetTexture = true;
//}


} // namespace voreen
