#include "Geom/Quad.h"

namespace SwwGeom {

Quad::Quad(const std::string &name):Mesh(name)
{
    Sww::Core::V3fArrayPtr vertices(new Sww::Core::V3fArray());
    Sww::Core::IntArrayPtr indices(new Sww::Core::IntArray());
    Sww::Core::V2fArrayPtr uvs(new Sww::Core::V2fArray());

    vertices->push_back(Imath::V3f(-1.f,-1.f,0.f));
    vertices->push_back(Imath::V3f(1.f,-1.f,0.f));
    vertices->push_back(Imath::V3f(1.f,1.f,0.f));
    vertices->push_back(Imath::V3f(-1.f,1.f,0.f));

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

    setVertices(vertices);
    setIndices(indices);
    setUvs(uvs);

}

}//ns
