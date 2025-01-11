#include "SpotLight.h"
#include "Geom/GL/GLLines.h"
#include "Util/SwwMath.h"

namespace SwwGeom {

SpotLight::SpotLight(const  std::string& name):Light(name),_angle(45)
{
}

bool SpotLight::updateGLResources(){
    if(!_glLines){
        _glLines.reset(new GLLines());

        float angleRad = Sww::Util::SwwMath::degToRad(_angle / 2.0);

        float coneHeight =  3;
        float coneRadius =  coneHeight * std::tan(angleRad);

        Sww::Core::V3fArrayPtr linesVertices(new Sww::Core::V3fArray());
        Sww::Core::IntArrayPtr counts(new Sww::Core::IntArray());

//        linesVertices->push_back(Imath::V3f(0.0,0.0,0.0));
//        linesVertices->push_back(Imath::V3f(coneHeight,coneRadius,0.0));
//        linesVertices->push_back(Imath::V3f(0.0,0.0,0.0));
//        linesVertices->push_back(Imath::V3f(coneHeight,0.0,coneRadius));
//        linesVertices->push_back(Imath::V3f(0.0,0.0,0.0));
//        linesVertices->push_back(Imath::V3f(coneHeight,-coneRadius,0.0));
//        linesVertices->push_back(Imath::V3f(0.0,0.0,0.0));
//        linesVertices->push_back(Imath::V3f(coneHeight,0.0,-coneRadius));
        linesVertices->push_back(Imath::V3f(0.0,0.0,0.0));
        linesVertices->push_back(Imath::V3f(coneRadius,0.0,-coneHeight));
        linesVertices->push_back(Imath::V3f(0.0,0.0,0.0));
        linesVertices->push_back(Imath::V3f(0.0,coneRadius,-coneHeight));
        linesVertices->push_back(Imath::V3f(0.0,0.0,0.0));
        linesVertices->push_back(Imath::V3f(-coneRadius,0.0,-coneHeight));
        linesVertices->push_back(Imath::V3f(0.0,0.0,0.0));
        linesVertices->push_back(Imath::V3f(0.0,-coneRadius,-coneHeight));
        counts->push_back(2);
        counts->push_back(2);
        counts->push_back(2);
        counts->push_back(2);

//        int devide = 16;
//        Imath::V3f v0(coneRadius,0.0,-coneHeight);
//        for(int i= 1 ;i <= devide;i++){
//            const float deg = 360.0 / (float)devide;
//            float theta = Sww::Util::SwwMath::degToRad(deg * i);
//            float vx = std::cos(theta) * coneRadius;
//            float vy = std::sin(theta) * coneRadius;

//            Imath::V3f v1(vx,vy,-coneHeight);
//            linesVertices->push_back(v0);
//            linesVertices->push_back(v1);
//            counts->push_back(2);
//            v0 = v1;
//        }

        int devide = 16;
        Imath::V3f v0(coneRadius,0.0,-coneHeight);
        linesVertices->push_back(v0);
        for(int i= 1 ;i <= devide;i++){
            const float deg = 360.0 / (float)devide;
            float theta = Sww::Util::SwwMath::degToRad(deg * i);
            float vx = std::cos(theta) * coneRadius;
            float vy = std::sin(theta) * coneRadius;

            Imath::V3f v1(vx,vy,-coneHeight);
            linesVertices->push_back(v1);
        }
        counts->push_back(devide + 1);

        Imath::V3f *vertices  =  &((*(linesVertices))[0]);
        size_t numVertices = linesVertices->size();

        GLenum usage = GL_STATIC_DRAW;

        _glLines->generate(vertices,numVertices,counts,usage);

    }
    return true;
}

}//ns
