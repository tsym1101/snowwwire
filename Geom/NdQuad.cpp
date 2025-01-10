#include "NdQuad.h"
#include "Core/PlugType.h"

namespace  SwwGeom{

NdQuad::NdQuad()
{
}

bool NdQuad::initialize()
{
    SWW_CHECK_BOOL(Transform::initialize());

    _min = new Sww::Core::V2fPlug("min",Imath::V2f(-1.f,-1.f));
    _min->setStorable(true);
    _min->setConnectable(true);

    _max = new Sww::Core::V2fPlug("max",Imath::V2f(1.f,1.f));
    _max->setStorable(true);
    _max->setConnectable(true);

    _oMesh = new SwwGeom::MeshPlug("oMesh");
    _oMesh->setConnectable(true);
    _oMesh->setOutput(true);

    addPlug(_min);
    addPlug(_max);
    addPlug(_oMesh);

    _parentTransform->affects(_oMesh);
    _translate->affects(_oMesh);
    _rotate->affects(_oMesh);
    _scale->affects(_oMesh);
    _min->affects(_oMesh);
    _max->affects(_oMesh);

    return true;
}
QString NdQuad::toolTip()const
{
    QString str = "your tooltip";
    return str;
}

Sww::Core::Node* NdQuad::creator(){
    return new NdQuad();
}

bool NdQuad::execute(const Sww::Core::PlugBase *plug)
{

    if(plug == _oMesh){
        //SWW_VAL(_flip,flip);

        if(!_meshPtr){

            SWW_VAL(_min,min);
            SWW_VAL(_max,max);

            Sww::Core::V3fArrayPtr vertices;
            Sww::Core::IntArrayPtr indices;
            Sww::Core::V2fArrayPtr uvs;

            vertices.reset(new Sww::Core::V3fArray());
            indices.reset(new Sww::Core::IntArray());
            uvs.reset(new Sww::Core::V2fArray());

            //            vertices->push_back(Imath::V3f(-1.f,-1.f,0.f));
            //            vertices->push_back(Imath::V3f(1.f,-1.f,0.f));
            //            vertices->push_back(Imath::V3f(1.f,1.f,0.f));
            //            vertices->push_back(Imath::V3f(-1.f,1.f,0.f));

            vertices->push_back(Imath::V3f(min.x,min.y,0.f));
            vertices->push_back(Imath::V3f(max.x,min.y,0.f));
            vertices->push_back(Imath::V3f(max.x,max.y,0.f));
            vertices->push_back(Imath::V3f(min.x,max.y,0.f));

            indices->push_back(0);
            indices->push_back(1);
            indices->push_back(2);
            indices->push_back(0);
            indices->push_back(2);
            indices->push_back(3);

            uvs->push_back(Imath::V2f(0.f,0.f));
            uvs->push_back(Imath::V2f(1.f,0.f));
            uvs->push_back(Imath::V2f(1.f,1.f));
            uvs->push_back(Imath::V2f(0.f,1.f));


            //SwwGeom::Mesh::Ptr mesh(new SwwGeom::Mesh("quad"));
            _meshPtr.reset(new SwwGeom::Mesh("quad"));
            _meshPtr->setVertices(vertices);
            _meshPtr->setIndices(indices);
            _meshPtr->setUvs(uvs);
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

Imath::Box3f NdQuad::getBounds() const{
    if(!_meshPtr){return Imath::Box3f();}
    return _meshPtr->getBounds();
}


}//ns OpenGL4

