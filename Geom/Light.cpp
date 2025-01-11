#include "Light.h"
#include "Geom/GL/GLLines.h"
#include "Util/SwwMath.h"

namespace SwwGeom {

Light::Light(const std::string &name):Shape(name)
{
}

bool Light::updateGLResources(){

    if(!_glLines){
        _glLines.reset(new GLLines());

        Sww::Core::V3fArrayPtr linesVertices(new Sww::Core::V3fArray());
        Sww::Core::IntArrayPtr counts(new Sww::Core::IntArray());

//        linesVertices->push_back(Imath::V3f(-0.5,0.0,0.0));
//        linesVertices->push_back(Imath::V3f(0.5,0.0,0.0));
//        linesVertices->push_back(Imath::V3f(0.0,0.0,-0.5));
//        linesVertices->push_back(Imath::V3f(0.0,0.0,0.5));
//        linesVertices->push_back(Imath::V3f(0.0,-0.5,0.0));
//        linesVertices->push_back(Imath::V3f(0.0,0.5,0.0));
//        counts->push_back(2);
//        counts->push_back(2);
//        counts->push_back(2);

        int devide = 16;
        float radius = 0.5;
        Imath::V3f v0;

        v0 = Imath::V3f(radius,0.0,0.0);
        linesVertices->push_back(v0);
        for(int i= 1 ;i <= devide;i++){
            const float deg = 360.0 / (float)devide;
            float theta = Sww::Util::SwwMath::degToRad(deg * i);
            float p0 = std::cos(theta) * radius;
            float p1 = std::sin(theta) * radius;
            Imath::V3f v1(p0,p1,0.0);
            linesVertices->push_back(v1);
        }
        //linesVertices->push_back(v0);

        v0 = Imath::V3f(0.0,0.0,-radius);
        linesVertices->push_back(v0);
        for(int i= 1 ;i <= devide;i++){
            const float deg = 360.0 / (float)devide;
            float theta = Sww::Util::SwwMath::degToRad(deg * i);
            float p0 = std::cos(theta) * radius;
            float p1 = std::sin(theta) * radius;
            Imath::V3f v1(0.0,p1,-p0);
            linesVertices->push_back(v1);
        }
        //linesVertices->push_back(v0);

        v0 = Imath::V3f(radius,0.0,0.0);
        linesVertices->push_back(v0);
        for(int i= 1 ;i <= devide;i++){
            const float deg = 360.0 / (float)devide;
            float theta = Sww::Util::SwwMath::degToRad(deg * i);
            float p0 = std::cos(theta) * radius;
            float p1 = std::sin(theta) * radius;
            Imath::V3f v1(p0,0.0,-p1);
            linesVertices->push_back(v1);
        }
        //linesVertices->push_back(v0);



        counts->push_back(devide + 1);
        counts->push_back(devide + 1);
        counts->push_back(devide + 1);

        Imath::V3f *vertices  =  &((*(linesVertices))[0]);
        size_t numVertices = linesVertices->size();

        GLenum usage = GL_STATIC_DRAW;

        _glLines->generate(vertices,numVertices,counts,usage);

    }

    return true;
}


bool Light::draw() const{

    Light* lightTmp = const_cast<Light*>(this);
    SWW_CHECK_BOOL(lightTmp->updateGLResources());

    _glLines->draw();

    return true;

}

bool Light::drawLegacy() const{

    Light* lightTmp = const_cast<Light*>(this);
    SWW_CHECK_BOOL(lightTmp->updateGLResources());

    _glLines->drawLegacy();

    return true;

}

Shape::Ptr Light::shallowCopy()const{
    SwwGeom::Light::Ptr out(new SwwGeom::Light(_name));
    *out = *this;
    return out;
}

}//ns
