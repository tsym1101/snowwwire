#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Geom/Light.h"

namespace SwwGeom {

class SpotLight : public  Light
{
public:

    SpotLight(const  std::string& name);
    virtual LightType getLightType()const{return kSpot;}
private:
    virtual bool updateGLResources();
    SWW_SIMPLE_MEMBER(float,angle)

};

}//ns

#endif // SPOTLIGHT_H
