#ifndef DirectionalLight_H
#define DirectionalLight_H
#include "Geom/Light.h"

namespace SwwGeom {

class DirectionalLight : public  Light
{
public:
    DirectionalLight(const  std::string& name);
    virtual LightType getLightType()const{return kDirectional;}
private:
    virtual bool updateGLResources();

};

}//ns
#endif // DirectionalLight_H
