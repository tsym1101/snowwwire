#include "Geom/Lines.h"
#include "Core/SwwAssert.h"

#include "Geom/GL/GLLines.h"
#include "Core/SwwMacro.h"

namespace SwwGeom {
Lines::Lines(const std::string &name):Shape(name),_flag(kNothing)
{
}

bool Lines::valid()const{
    return _linesVertices && _counts;
}

void  Lines::setVertices(const Sww::Core::V3fArrayPtr& ptr){
    _linesVertices = ptr;
    _flag |= kMustUpdateVerts;
}
void  Lines::setNormals(const Sww::Core::V3fArrayPtr& ptr){
    _linesNormals  = ptr;
    _flag |= kMustUpdateNormals;
}
void  Lines::setUvs(const Sww::Core::V2fArrayPtr& ptr){
    _linesUvs  = ptr;
    _flag |= kMustUpdateUvs;
}
void  Lines::setColors(const Sww::Core::V3fArrayPtr& ptr){
    _linesColors  = ptr;
    _flag |= kMustUpdateColors;
}
void  Lines::setAlphas(const Sww::Core::FloatArrayPtr& ptr){
    _linesAlphas = ptr;
    _flag |= kMustUpdateAlphas;
}

size_t Lines::getNumVertices()const{
    if(!_linesVertices){
        SWW_ERR("invalid lines.");
        return 0;
    }
    return _linesVertices->size();
}


size_t Lines::getNumNormals()const{
    if(!_linesNormals){
        SWW_ERR("invalid normals.");
        return 0;
    }
    return _linesNormals->size();
}

size_t Lines::getNumUvs()const{
    if(!_linesUvs){
        SWW_ERR("invalid normals.");
        return 0;
    }
    return _linesUvs->size();
}

size_t Lines::getNumColors()const{
    if(!_linesColors){
        SWW_ERR("invalid colors.");
        return 0;
    }
    return _linesColors->size();
}

size_t Lines::getNumAlphas()const{
    if(!_linesAlphas){
        SWW_ERR("invalid alphas.");
        return 0;
    }
    return _linesAlphas->size();
}


Lines::Ptr Lines::deepCopy() const{

    if(!valid()){
        SWW_ERR("invalid lines. copy failed.");
        return Lines::Ptr();
    }

    Sww::Core::V3fArrayPtr oLinesP;

    Lines::Ptr oLinesPtr(new Lines(_name + "_clone"));

    oLinesPtr->setBounds(_bbox);
    oLinesPtr->setTransform(_iTransform);

    oLinesP.reset(new Sww::Core::V3fArray(_linesVertices->size()));

    memcpy(  &((*oLinesP)[0]) ,(void*)&((*_linesVertices)[0]) , sizeof(Imath::V3f) * _linesVertices->size() );

    oLinesPtr->setVertices(oLinesP);

    if(_counts){
        Sww::Core::IntArrayPtr oFaceCounts;
        oFaceCounts.reset(new Sww::Core::IntArray(_counts->size()));
        memcpy( &((*oFaceCounts)[0]) ,(void*)&((*_counts)[0]) , sizeof(int) * _counts->size() );
        oLinesPtr->setCounts(oFaceCounts);
    }

    if(_linesNormals){
        Sww::Core::V3fArrayPtr oLinesNormals;
        oLinesNormals.reset(new Sww::Core::V3fArray(_linesNormals->size()) );
        memcpy(  &((*oLinesNormals)[0]) ,(void*)&((*_linesNormals)[0]) , sizeof(Imath::V3f) * _linesNormals->size() );
        oLinesPtr->setNormals(oLinesNormals);

    }

    if(_linesUvs){
        Sww::Core::V2fArrayPtr oLinesUvs;
        oLinesUvs.reset(new Sww::Core::V2fArray(_linesUvs->size()) );
        memcpy(  &((*oLinesUvs)[0]) ,(void*)&((*_linesUvs)[0]) , sizeof(Imath::V2f) * _linesUvs->size() );
        oLinesPtr->setUvs(oLinesUvs);

    }

    if(_linesColors || _linesAlphas){

        if(_linesColors){
            Sww::Core::V3fArrayPtr oLinesColors;
            oLinesColors.reset(new Sww::Core::V3fArray(_linesColors->size()) );
            memcpy(  &((*oLinesColors)[0]) ,(void*)&((*_linesColors)[0]) , sizeof(Imath::V3f) * _linesColors->size() );
            oLinesPtr->setColors(oLinesColors);
        }

        if(_linesAlphas){
            Sww::Core::FloatArrayPtr oLinesAlphas;
            oLinesAlphas.reset(new Sww::Core::FloatArray(_linesAlphas->size()) );
            memcpy(  &((*oLinesAlphas)[0]) ,(void*)&((*_linesAlphas)[0]) , sizeof(float) * _linesAlphas->size() );
            oLinesPtr->setAlphas(oLinesAlphas);
        }

    }

    return oLinesPtr;
}

void Lines::printVertices()const{
    Q_ASSERT(_linesVertices);
    for(int i = 0;i < _linesVertices->size();i++){
        SWW_DEBUG("pt[" << i << "]=" << (*_linesVertices)[i]);
    }
}

bool Lines::updateGLResources(){

    if(!valid()){
        SWW_ERR("invalid lines. " << getName());
        return false;
    }

    if(!_glLines){
        _glLines.reset(new GLLines());

        Lines* linesPtr = this;

        Imath::V3f *vertices  =  &((*(linesPtr->getVertices()))[0]);
        size_t numVertices = linesPtr->getVertices()->size();

        GLenum usage = GL_STATIC_DRAW;
        if(isDynamic())usage = GL_DYNAMIC_DRAW;

        _glLines->generate(vertices,numVertices,_counts,usage);

        if(linesPtr->hasNormals()){
            Imath::V3f *normals = &((*(linesPtr->getNormals()))[0]);
            size_t numNormals = linesPtr->getNormals()->size();
            _glLines->generateNormals(normals,numNormals);
        }
        if(linesPtr->hasUvs()){
            Imath::V2f *uvs = &((*(linesPtr->getUvs()))[0]);
            size_t numUvs = linesPtr->getUvs()->size();
            _glLines->generateUvs(uvs,numUvs);
        }
        if(linesPtr->hasColors()){
            Imath::V3f *colors = &((*(linesPtr->getColors()))[0]);
            size_t numColors = linesPtr->getColors()->size();
            _glLines->generateColors(colors,numColors);
        }
        if(linesPtr->hasAlphas()){
            float *alphas = &((*(linesPtr->getAlphas()))[0]);
            size_t numAlphas = linesPtr->getAlphas()->size();
            _glLines->generateAlphas(alphas,numAlphas);
        }
    }
    else if(isDynamic() || _flag){

        if(_flag == kNothing){
            return true;
        }

        Lines* linesPtr = this;

        if(_flag & kMustUpdateVerts){
            Imath::V3f *vertices  =  &((*(linesPtr->getVertices()))[0]);
            size_t numVertices = linesPtr->getVertices()->size();
            _glLines->update(vertices,numVertices);
            _glLines->setCountsArray(_counts);

            //_flag & ~kMustUpdateVerts;//フラグ解除 ビット反転して論理積
        }
        if(_flag & kMustUpdateNormals){
            Imath::V3f *normals = &((*(linesPtr->getNormals()))[0]);
            size_t numNormals = linesPtr->getNormals()->size();
            _glLines->updateNormals(normals,numNormals);

            //_flag & ~kMustUpdateNormals;//フラグ解除
        }
        if(_flag & kMustUpdateUvs){
            Imath::V2f *uvs = &((*(linesPtr->getUvs()))[0]);
            size_t numUvs = linesPtr->getUvs()->size();
            _glLines->updateUvs(uvs,numUvs);

            //_flag & ~kMustUpdateUvs;//フラグ解除
        }
        if(_flag & kMustUpdateColors){
            Imath::V3f *colors = &((*(linesPtr->getColors()))[0]);
            size_t numColors = linesPtr->getColors()->size();
            _glLines->updateColors(colors,numColors);

            //_flag & ~kMustUpdateColors;//フラグ解除
        }
        if(_flag & kMustUpdateAlphas){
            float *alphas = &((*(linesPtr->getAlphas()))[0]);
            size_t numAlphas = linesPtr->getAlphas()->size();
            _glLines->updateAlphas(alphas,numAlphas);

            //_flag & ~kMustUpdateColors;//フラグ解除
        }
    }

    _flag = kNothing;

    return true;
}

bool Lines::draw() const{

    Lines* linesTmp = const_cast<Lines*>(this);
    SWW_CHECK_BOOL(linesTmp->updateGLResources());

    _glLines->draw();

    return true;

}

bool Lines::drawLegacy() const{

    Lines* linesTmp = const_cast<Lines*>(this);
    SWW_CHECK_BOOL(linesTmp->updateGLResources());

    _glLines->drawLegacy();

    return true;

}

Shape::Ptr Lines::shallowCopy()const{
    SwwGeom::Lines::Ptr out(new SwwGeom::Lines(_name));
    *out = *this;
    return out;
}

}//ns
