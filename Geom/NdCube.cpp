#include "NdCube.h"
#include "Core/PlugType.h"

namespace  SwwGeom{

NdCube::NdCube()
{
}

bool NdCube::initialize()
{
    SWW_CHECK_BOOL(Transform::initialize());

    _oMesh = new SwwGeom::MeshPlug("oMesh");
    _oMesh->setConnectable(true);
    _oMesh->setOutput(true);

    addPlug(_oMesh);

    _parentTransform->affects(_oMesh);
    _translate->affects(_oMesh);
    _rotate->affects(_oMesh);
    _scale->affects(_oMesh);

    return true;
}
QString NdCube::toolTip()const
{
    QString str = "your tooltip";
    return str;
}

Sww::Core::Node* NdCube::creator(){
    return new NdCube();
}

bool NdCube::execute(const Sww::Core::PlugBase *plug)
{

    if(plug == _oMesh){

        if(!_meshPtr){

            Sww::Core::V3fArrayPtr vertices;
            Sww::Core::IntArrayPtr indices;

            vertices.reset(new Sww::Core::V3fArray());
            indices.reset(new Sww::Core::IntArray());

            vertices->push_back(Imath::V3f(-0.5 ,-0.5 ,0.5));
            vertices->push_back(Imath::V3f(0.5 ,-0.5 ,0.5));
            vertices->push_back(Imath::V3f(-0.5 ,0.5 ,0.5));
            vertices->push_back(Imath::V3f(0.5 ,0.5 ,0.5));
            vertices->push_back(Imath::V3f(-0.5 ,0.5 ,-0.5));
            vertices->push_back(Imath::V3f(0.5 ,0.5 ,-0.5));
            vertices->push_back(Imath::V3f(-0.5 ,-0.5 ,-0.5));
            vertices->push_back(Imath::V3f(0.5 ,-0.5 ,-0.5));

            indices->push_back(2);
            indices->push_back(3);
            indices->push_back(0);
            indices->push_back(4);
            indices->push_back(3);
            indices->push_back(2);
            indices->push_back(6);
            indices->push_back(7);
            indices->push_back(4);
            indices->push_back(0);
            indices->push_back(7);
            indices->push_back(6);
            indices->push_back(3);
            indices->push_back(5);
            indices->push_back(1);
            indices->push_back(4);
            indices->push_back(0);
            indices->push_back(6);
            indices->push_back(3);
            indices->push_back(1);
            indices->push_back(0);
            indices->push_back(5);
            indices->push_back(3);
            indices->push_back(4);
            indices->push_back(7);
            indices->push_back(5);
            indices->push_back(4);
            indices->push_back(1);
            indices->push_back(7);
            indices->push_back(0);
            indices->push_back(2);
            indices->push_back(0);
            indices->push_back(4);
            indices->push_back(5);
            indices->push_back(7);
            indices->push_back(1);

            _meshPtr.reset(new SwwGeom::Mesh("Cube"));
            _meshPtr->setVertices(vertices);
            _meshPtr->setIndices(indices);
            _meshPtr->computeBounds(vertices);

            _oMesh->setOutputValue(_meshPtr);

        }

        Imath::M44f t;
        SWW_CHECK_BOOL(getTransform(t));

        _meshPtr->setTransform(t);
        return true;
    }

    SWW_CHECK_BOOL(Transform::initialize());

    return true;
}

Imath::Box3f NdCube::getBounds() const{
    if(!_meshPtr){return Imath::Box3f();}
    return _meshPtr->getBounds();
}

}//ns OpenGL4

