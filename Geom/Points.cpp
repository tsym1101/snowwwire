#include "Points.h"

#include "Core/SwwAssert.h"
#include "Geom/GL/GLPoints.h"
#include "Core/SwwMacro.h"

namespace SwwGeom {

Points::Points(const std::string &name):Shape(name),_flag(kNothing)
{
}

bool Points::valid()const{
    return (bool)_meshVertices.get();
}

void  Points::setVertices(const Sww::Core::V3fArrayPtr& ptr){
    _meshVertices = ptr;
    _flag |= kMustUpdateVerts;
}
void  Points::setNormals(const Sww::Core::V3fArrayPtr& ptr){
    _meshNormals  = ptr;
    _flag |= kMustUpdateNormals;
}
void  Points::setUvs(const Sww::Core::V2fArrayPtr& ptr){
    _meshUvs  = ptr;
    _flag |= kMustUpdateUvs;
}
void  Points::setColors(const Sww::Core::V3fArrayPtr& ptr){
    _meshColors  = ptr;
    _flag |= kMustUpdateColors;
}
void  Points::setAlphas(const Sww::Core::FloatArrayPtr& ptr){
    _meshAlphas = ptr;
}


Points::Ptr Points::deepCopy() const{

    if(!valid()){
        SWW_ERR("invalid mesh. copy failed.");
        return Points::Ptr();
    }

    Sww::Core::V3fArrayPtr oMeshP;

    Points::Ptr oPointsPtr(new Points(_name + "_clone"));

    oPointsPtr->setBounds(_bbox);
    oPointsPtr->setTransform(_iTransform);

    oMeshP.reset(new Sww::Core::V3fArray(_meshVertices->size()));

    memcpy(  &((*oMeshP)[0]) ,(void*)&((*_meshVertices)[0]) , sizeof(Imath::V3f) * _meshVertices->size() );

    oPointsPtr->setVertices(oMeshP);

    if(_meshNormals){
        Sww::Core::V3fArrayPtr oMeshNormals;
        oMeshNormals.reset(new Sww::Core::V3fArray(_meshNormals->size()) );
        oPointsPtr->setNormals(oMeshNormals);
    }

    if(_meshUvs){
        Sww::Core::V2fArrayPtr oMeshUvs;
        oMeshUvs.reset(new Sww::Core::V2fArray(_meshUvs->size()) );
        memcpy(  &((*oMeshUvs)[0]) ,(void*)&((*_meshUvs)[0]) , sizeof(Imath::V2f) * _meshUvs->size() );
        oPointsPtr->setUvs(oMeshUvs);
    }

    if(_meshColors || _meshAlphas){

        if(_meshColors){
            Sww::Core::V3fArrayPtr oMeshColors;
            oMeshColors.reset(new Sww::Core::V3fArray(_meshColors->size()) );
            memcpy(  &((*oMeshColors)[0]) ,(void*)&((*_meshColors)[0]) , sizeof(Imath::V3f) * _meshColors->size() );
            oPointsPtr->setColors(oMeshColors);
        }

        if(_meshAlphas){
            Sww::Core::FloatArrayPtr oMeshAlphas;
            oMeshAlphas.reset(new Sww::Core::FloatArray(_meshAlphas->size()) );
            memcpy(  &((*oMeshAlphas)[0]) ,(void*)&((*_meshAlphas)[0]) , sizeof(float) * _meshAlphas->size() );
            oPointsPtr->setAlphas(oMeshAlphas);
        }
    }

    return oPointsPtr;
}

void Points::printVertices()const{
    Q_ASSERT(_meshVertices);
    for(int i = 0;i < _meshVertices->size();i++){
        SWW_DEBUG("pt[" << i << "]=" << (*_meshVertices)[i]);
    }
}

bool Points::draw() const{
    Points* tmp = const_cast<Points*>(this);
    SWW_CHECK_BOOL(tmp->updateGLResources());

    _glPoints->draw();

    return true;
}

bool Points::drawLegacy() const{

    Points* tmp = const_cast<Points*>(this);
    SWW_CHECK_BOOL(tmp->updateGLResources());

    _glPoints->drawLegacy();

    return true;

}

bool Points::updateGLResources(){

    if(!_glPoints){
        _glPoints.reset(new GLPoints());

        Points* pointsPtr = this;

        Imath::V3f *vertices  =  &((*(pointsPtr->getVertices()))[0]);
        size_t numVertices = pointsPtr->getVertices()->size();

        GLenum usage = GL_STATIC_DRAW;
        if(isDynamic())usage = GL_DYNAMIC_DRAW;

        _glPoints->generate(vertices,numVertices,usage);

        if(pointsPtr->hasNormals()){
            Imath::V3f *normals = &((*(pointsPtr->getNormals()))[0]);
            size_t numNormals = pointsPtr->getNormals()->size();
            _glPoints->generateNormals(normals,numNormals);
        }
        if(pointsPtr->hasUvs()){
            Imath::V2f *uvs = &((*(pointsPtr->getUvs()))[0]);
            size_t numUvs = pointsPtr->getUvs()->size();
            _glPoints->generateUvs(uvs,numUvs);
        }
        if(pointsPtr->hasColors()){
            Imath::V3f *colors = &((*(pointsPtr->getColors()))[0]);
            size_t numColors = pointsPtr->getColors()->size();
            _glPoints->generateColors(colors,numColors);
        }
    }
    else if(isDynamic() || _flag){

        if(_flag == kNothing){
            return true;
        }

        Points* pointsPtr = this;

        if(_flag & kMustUpdateVerts){
            Imath::V3f *vertices  =  &((*(pointsPtr->getVertices()))[0]);
            size_t numVertices = pointsPtr->getVertices()->size();
            _glPoints->update(vertices,numVertices);

            //_flag & ~kMustUpdateVerts;//フラグ解除 ビット反転して論理積
        }
        if(_flag & kMustUpdateNormals){
            Imath::V3f *normals = &((*(pointsPtr->getNormals()))[0]);
            size_t numNormals = pointsPtr->getNormals()->size();
            _glPoints->updateNormals(normals,numNormals);

            //_flag & ~kMustUpdateNormals;//フラグ解除
        }
        if(_flag & kMustUpdateUvs){
            Imath::V2f *uvs = &((*(pointsPtr->getUvs()))[0]);
            size_t numUvs = pointsPtr->getUvs()->size();
            _glPoints->updateUvs(uvs,numUvs);

            //_flag & ~kMustUpdateUvs;//フラグ解除
        }
        if(_flag & kMustUpdateColors){
            Imath::V3f *colors = &((*(pointsPtr->getColors()))[0]);
            size_t numColors = pointsPtr->getColors()->size();
            _glPoints->updateColors(colors,numColors);

            //_flag & ~kMustUpdateColors;//フラグ解除
        }
    }

    _flag = kNothing;

    return true;
}


Shape::Ptr Points::shallowCopy()const{
    SwwGeom::Points::Ptr out(new SwwGeom::Points(_name));
    *out = *this;
    return out;
}

const SwwGeom::ArrayProperty::Ptr Points::getProperty(const std::string& propName)const{
    return _ArrayPropertys.value(propName);
}

void Points::addProperty(const SwwGeom::ArrayProperty::Ptr& prop){
    _ArrayPropertys.push_back(prop);
}

bool Points::removeProperty(const std::string& propName){
    if(!_ArrayPropertys.contains(propName))return true;
    _ArrayPropertys.erase(propName);
    return false;
}

void Points::clearProperty(){
    _ArrayPropertys.clear();
}

std::vector<std::string> Points::getPropertyNames()const{
    return _ArrayPropertys.keys();
}

bool Points::contains(const std::string &propName) const{
    return _ArrayPropertys.contains(propName);
}



}//ns
