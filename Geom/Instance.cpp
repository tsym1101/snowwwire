#include "Geom/Instance.h"
#include "ImathBoxAlgo.h"

#include "Core/SwwAssert.h"
#include "Core/SwwMacro.h"
#include "Geom/GL/GLInstance.h"
#include "Geom/Mesh.h"

namespace SwwGeom {

Instance::Instance(const std::string& name):Shape(name),_baseMesh(0),
    _mustUpdateInstance(false),_mustUpdateMagnitude(false),_mustUpdateMesh(false)
{
}

bool Instance::draw()const{
    Instance* tmp = const_cast<Instance*>(this);
    SWW_CHECK_BOOL(tmp->updateGLResources());
    _glInstance->draw();

    return true;
}

bool Instance::drawLegacy()const{
    SWW_ERR("draw instance not supported in gl legacy renderer. " << getName());
    return false;
}

Shape::Ptr Instance::shallowCopy()const{
    SwwGeom::Instance::Ptr out(new SwwGeom::Instance(_name));
    *out = *this;
    return out;
}

void Instance::setMatArray(const Sww::Core::M44fArrayPtr& matArray){
    _matArray = matArray;
    _mustUpdateInstance = true;
}

void Instance::setMagArray(const Sww::Core::FloatArrayPtr& magArray){
    _magArray = magArray;
    _mustUpdateMagnitude = true;
}

//void Instance::setMatArray(Imath::M44f* matArray,size_t size){
//    _matArray = matArray;
//    _mustUpdateInstance = true;
//}

//void Instance::setMagArray(float* magArray,size_t size){
//    _magArray = magArray;
//    _mustUpdateMagnitude = true;
//}

void Instance::setBaseMesh(const Mesh* baseMesh){
    if(_baseMesh != baseMesh){
        _mustUpdateMesh = true;
    }
    _baseMesh = baseMesh;
}

bool Instance::updateGLResources(){

    if(!_baseMesh){
        SWW_ERR("baseMesh is null." << getName());
        return false;
    }

    if(!_matArray){
        SWW_ERR("matArray is null." << getName());
        return false;
    }

    if(!_glInstance || _mustUpdateMesh){
        Mesh* tmp = const_cast<Mesh*>(_baseMesh);
        if(!tmp->updateGLResources()){
            SWW_ERR("baseMesh resources invalid. " << _baseMesh->getName() <<
                   " in " << getName());
            return false;
        }

        Imath::M44f *matArray  =  &((*_matArray)[0]);

        _glInstance.reset(new GLInstance());
        _glInstance->generateInstance(_baseMesh->_glMesh.get(),
                                      matArray,
                                      _matArray->size(),
                                      GL_STATIC_DRAW);

        if(_magArray){
            float* magArray  =  &((*_magArray)[0]);
            _glInstance->generateMagnitude(magArray,_magArray->size());
            _mustUpdateMagnitude = false;
        }

        _mustUpdateInstance = false;
        _mustUpdateMesh = false;
    }
    else{        
        if(_mustUpdateInstance){
            Imath::M44f *matArray  =  &((*_matArray)[0]);
            _glInstance->updateInstance(matArray,_matArray->size());
            _mustUpdateInstance = false;
        }

        if(_mustUpdateMagnitude){
            float* magArray  =  &((*_magArray)[0]);
            _glInstance->generateMagnitude(magArray,_magArray->size());
            _mustUpdateMagnitude = false;
        }

    }

    return true;
}

}//ns
