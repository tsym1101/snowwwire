#include "Cube.h"

namespace SwwGeom{

Cube::Cube(const std::string &name):Mesh(name)
{
    Sww::Core::V3fArrayPtr vertices(new Sww::Core::V3fArray());
    Sww::Core::IntArrayPtr indices(new Sww::Core::IntArray());

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

    setVertices(vertices);
    setIndices(indices);

}

}//ns
