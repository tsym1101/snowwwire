#include "DirectionalLight.h"
#include "Geom/GL/GLLines.h"

namespace SwwGeom {

DirectionalLight::DirectionalLight(const  std::string& name):Light(name)
{

}

bool DirectionalLight::updateGLResources(){
    if(!_glLines){
        _glLines.reset(new GLLines());

        Sww::Core::V3fArrayPtr linesVertices(new Sww::Core::V3fArray());
        Sww::Core::IntArrayPtr counts(new Sww::Core::IntArray());

//        linesVertices->push_back(Imath::V3f(-1.0,0.0,0.0));
//        linesVertices->push_back(Imath::V3f(1.0,0.0,0.0));
//        linesVertices->push_back(Imath::V3f(0.8,0.2,0.0));
//        linesVertices->push_back(Imath::V3f(1.0,0.0,0.0));
//        linesVertices->push_back(Imath::V3f(0.8,0.0,0.2));
//        linesVertices->push_back(Imath::V3f(1.0,0.0,0.0));
//        linesVertices->push_back(Imath::V3f(0.8,-0.2,0.0));
//        linesVertices->push_back(Imath::V3f(1.0,0.0,0.0));
//        linesVertices->push_back(Imath::V3f(0.8,0.0,-0.2));
//        linesVertices->push_back(Imath::V3f(1.0,0.0,0.0));
        linesVertices->push_back(Imath::V3f(0.0,0.0,-1.0));
        linesVertices->push_back(Imath::V3f(0.0,0.0,1.0));
        linesVertices->push_back(Imath::V3f(0.0,0.2,-0.8));
        linesVertices->push_back(Imath::V3f(0.0,0.0,-1.0));
        linesVertices->push_back(Imath::V3f(-0.2,0.0,-0.8));
        linesVertices->push_back(Imath::V3f(0.0,0.0,-1.0));
        linesVertices->push_back(Imath::V3f(0.0,-0.2,-0.8));
        linesVertices->push_back(Imath::V3f(0.0,0.0,-1.0));
        linesVertices->push_back(Imath::V3f(0.2,0.0,-0.8));
        linesVertices->push_back(Imath::V3f(0.0,0.0,-1.0));

        counts->push_back(2);
        counts->push_back(2);
        counts->push_back(2);
        counts->push_back(2);
        counts->push_back(2);

        Imath::V3f *vertices  =  &((*(linesVertices))[0]);
        size_t numVertices = linesVertices->size();

        GLenum usage = GL_STATIC_DRAW;

        _glLines->generate(vertices,numVertices,counts,usage);

    }
    return true;
}

}//ns
