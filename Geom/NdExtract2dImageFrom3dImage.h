#ifndef SWW_NDEXTRACT2DIMAGEFROM3DIMAGE_H
#define SWW_NDEXTRACT2DIMAGEFROM3DIMAGE_H

#include "Core/Node.h"
#include "Node/PlugTypeDefFwd.h"

namespace SwwGeom {

class FloatImagePlug;

class  NdExtract2dImageFrom3dImage :public Sww::Core::Node
{
public:
    NdExtract2dImageFrom3dImage();
    virtual ~NdExtract2dImageFrom3dImage(){}
    virtual bool initialize();
    virtual QString toolTip() const;
    virtual bool execute(const Sww::Core::PlugBase *plug);
    static Sww::Core::Node* creator();

private:
    SwwGeom::FloatImagePlug* _i3dImage;
    Sww::Core::IntPlug* _z;
    SwwGeom::FloatImagePlug* _o2dImage;

};

}//ns SwwGeom

#endif // SWW_NDEXTRACT2DIMAGEFROM3DIMAGE_H
