#ifndef SWW_NdDirectionalLight_H
#define SWW_NdDirectionalLight_H

#include "Core/Node.h"
#include "Node/PlugTypeDefFwd.h"
#include "Geom/NdPointLight.h"

namespace SwwGeom {

class  NdDirectionalLight :public NdPointLight
{
public:
    NdDirectionalLight();
    virtual ~NdDirectionalLight(){}
    virtual bool initialize();
    virtual QString toolTip() const;
    virtual bool execute(const Sww::Core::PlugBase *plug);
    static Sww::Core::Node* creator();
};

}//ns SwwGeom

#endif // SWW_NdDirectionalLight_H
