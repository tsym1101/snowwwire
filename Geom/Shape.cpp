#include "Geom/Shape.h"
#include "ImathBoxAlgo.h"

#include "Core/SwwAssert.h"



namespace SwwGeom {


Shape::Shape(const std::string& name):_name(name),_isDynamic(false)
{
}

void Shape::computeBounds(const Sww::Core::V3fArrayConstPtr& posArray){
    _bbox.makeEmpty();
    if ( posArray )
    {
        size_t numPoints = posArray->size();
        for ( size_t p = 0; p < numPoints; ++p )
        {
            const Imath::V3f &P = (*posArray)[p];
            _bbox.extendBy( Imath::V3f( P.x, P.y, P.z ) );
        }
    }
}

Imath::Box3f Shape::getBounds()const{
    if(_bbox.isEmpty()){
        SWW_ERR("invalid bounds. " << getName());
        return Imath::Box3f();
    }
    Imath::Box3f bbox = Imath::transform(_bbox,_iTransform);
    return bbox;
}

Imath::V3f Shape::getDirection()const{
    //Imath::V4f base(1.0,0.0,0.0,0.0);//a=0 for remove translate.
    Imath::V4f base(0.0,0.0,-1.0,0.0);//a=0 for remove translate.
    base = base * _iTransform;
    Imath::V3f result = Imath::V3f(base.x,base.y,base.z);
    result.normalize();

    return result;
}


}//ns
