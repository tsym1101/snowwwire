#include "NdTransformGeo.h"
#include "Geom/PlugTypeGeom.h"
#include <QElapsedTimer>

namespace  SwwGeom{

NdTransformGeo::NdTransformGeo()
{
}

bool NdTransformGeo::initialize()
{
    SWW_CHECK_BOOL(SwwGeom::Transform::initialize());

    _iShape = new SwwGeom::ShapePlug("iShape");
    _iShape->setConnectable(true);

    _oShape = new SwwGeom::ShapePlug("oShape");
    _oShape->setOutput(true);
    _oShape->setConnectable(true);

    addPlug(_iShape);
    addPlug(_oShape);

    _iShape->affects(_oShape);
    _translate->affects(_oShape);
    _rotate->affects(_oShape);
    _scale->affects(_oShape);
    _parentTransform->affects(_oShape);

    return true;
}
QString NdTransformGeo::toolTip()const
{
    QString str = "your tooltip";
    return str;
}

Sww::Core::Node* NdTransformGeo::creator(){
    return new NdTransformGeo();
}

bool NdTransformGeo::execute(const Sww::Core::PlugBase *plug)
{
    if(plug == _oShape){
        SWW_VAL(_iShape,shape);

        if(!shape){
            SWW_ERR("shape empty." << plug->getDebugName());
            return false;
        }

        Imath::M44f thisT;
        SWW_CHECK_BOOL(getTransform(thisT));

        QElapsedTimer timer;
        timer.start();
        SwwGeom::Shape::Ptr oShapePtr = shape->shallowCopy();
        size_t elapsed = timer.elapsed();
        SWW_DEBUG_VALUE(elapsed);

        oShapePtr->applyTransform(thisT);
        _oShape->setOutputValue(oShapePtr);

        return true;
    }

    SWW_CHECK_BOOL(SwwGeom::Transform::execute(plug));
    return true;
}

}//ns SwwGeom
