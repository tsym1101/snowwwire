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

bool TransFunc::mustResetTexture()const{
    return _mustResetTexture;
}


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

} // namespace voreen
