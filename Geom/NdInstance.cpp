#include "NdInstance.h"
#include "Geom/PlugTypeGeom.h"
#include "Core/PlugType.h"
#include <ImathBoxAlgo.h>
#include <ImathMatrixAlgo.h>

#include <glm/gtc/matrix_transform.hpp>


namespace  SwwGeom{

NdInstance::NdInstance()
{
}

bool NdInstance::initialize()
{
    SWW_CHECK_BOOL(Transform::initialize());

    _iMeshPlug = new SwwGeom::MeshPlug("iMesh");
    _iMeshPlug->setConnectable(true);

    _iPointsPlug = new SwwGeom::PointsPlug("iPoints");
    _iPointsPlug->setConnectable(true);

    _instancePlug= new SwwGeom::InstancePlug("instance");
    _instancePlug->setConnectable(true);
    _instancePlug->setOutput(true);

    addPlug(_iMeshPlug);
    addPlug(_iPointsPlug);
    addPlug(_instancePlug);

    _parentTransform->affects(_instancePlug);
    _translate->affects(_instancePlug);
    _rotate->affects(_instancePlug);
    _scale->affects(_instancePlug);
    _iMeshPlug->affects(_instancePlug);
    _iPointsPlug->affects(_instancePlug);
    _instance.reset(new SwwGeom::Instance(getName()));

    return true;
}

QString NdInstance::toolTip()const
{
    QString str = "your tooltip";
    return str;
}

Sww::Core::Node* NdInstance::creator(){
    return new NdInstance();
}

bool NdInstance::execute(const Sww::Core::PlugBase *plug)
{
    SWW_VAL(_iMeshPlug,iMesh);
    if(!iMesh){
        SWW_ERR("empty mesh. " << plug->getDebugName());
        _bounds.makeEmpty();
        return false;
    }

    SWW_VAL(_iPointsPlug,iPoints);
    if(!iPoints){
        SWW_ERR("empty points. " << plug->getDebugName());
        _bounds.makeEmpty();
        return false;
    }

    const ArrayProperty::Ptr arrayPropPtr = iPoints->getProperty("magnitude");
    if(arrayPropPtr && arrayPropPtr->type() == ArrayProperty::kFloat){
        const FloatArrayProperty::Ptr magArrayPropPtr = ArrayPropertyCast::toFloatArray(arrayPropPtr);
        Q_ASSERT(magArrayPropPtr);
        _instance->setMagArray(magArrayPropPtr->getArray());
    }

    const ArrayProperty::Ptr arrayPropPtr1 = iPoints->getProperty("velocity");
    V3fArrayProperty::Ptr velArrayPropPtr;
    if(arrayPropPtr1 && arrayPropPtr1->type() == ArrayProperty::kV3f){
        velArrayPropPtr = ArrayPropertyCast::toV3fArray(arrayPropPtr1);
        Q_ASSERT(velArrayPropPtr);
    }

    const Sww::Core::V3fArrayPtr& iPosArray = iPoints->getVertices();
    Sww::Core::M44fArrayPtr matArray(new Sww::Core::M44fArray(iPosArray->size()));
    int i = 0;
    for(auto p:*iPosArray){
        const Imath::M44f& meshT = iMesh->getTransform();
        Imath::M44f m;
        m.translate(p);

        if(velArrayPropPtr && i < velArrayPropPtr->getArray()->size()){
            Imath::V3f upVec(0,1,0);
            Imath::V3f from(0,0,0);
            const Imath::V3f& to = (*(velArrayPropPtr->getArray()))[i];
            //Imath::M44f rotMat = Imath::rotationMatrixWithUpDir(from,to,upVec);
            glm::mat4 matTmp = glm::lookAt( *((glm::vec3*)(&from)),*((glm::vec3*)(&to)),*((glm::vec3*)(&upVec)));
            Imath::M44f rotMat = *((Imath::M44f*)(&matTmp));
            rotMat = rotMat.inverse();
            m =  *((Imath::M44f*)(&matTmp)) * m;
        }

        (*matArray)[i] =  meshT * m;
        i++;
    }

    _instance->setMatArray(matArray);
    _instance->setBaseMesh(iMesh.get());


    Imath::M44f t;
    SWW_CHECK_BOOL(getTransform(t));
    _instance->setTransform(t);
    _bounds = iPoints->getBounds();

    _bounds = Imath::transform(_bounds,t);
    _instancePlug->setOutputValue(_instance);

    return true;
}

Imath::Box3f NdInstance::getBounds() const{
    return _bounds;
}

}//ns SwwGeom
