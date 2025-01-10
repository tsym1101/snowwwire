#ifndef SWW_NDEXTRACTVP_H
#define SWW_NDEXTRACTVP_H

#include "Core/Node.h"
#include "Node/PlugTypeDefFwd.h"
#include <ImathMatrix.h>.h>

namespace SwwGeom {

class CameraPlug;

class  NdExtractVP :public Sww::Core::Node
{
public:
    NdExtractVP();
    virtual ~NdExtractVP(){}
    virtual bool initialize();
    virtual QString toolTip() const;
    virtual bool execute(const Sww::Core::PlugBase *plug);
    static Sww::Core::Node* creator();
private:
    CameraPlug* _camera;
    Sww::Core::V2fPlug* _deviceAspect;
    Sww::Core::M44fPlug* _V;
    Sww::Core::M44fPlug* _P;
    Sww::Core::M44fPlug* _VP;


    bool getV(Imath::M44f &m);
    bool getP(Imath::M44f &m);
    bool getVP();
};

}//ns SwwGeom

#endif // SWW_NDEXTRACTVP_H
