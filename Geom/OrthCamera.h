#ifndef SWW_ORTHCAMERA_H
#define SWW_ORTHCAMERA_H

#include "Geom/Camera.h"
#include "Node/PlugTypeDefFwd.h"

namespace SwwGeom {

class  OrthCamera :public SwwGeom::Camera
{
public:
    OrthCamera();
    virtual ~OrthCamera(){}
    virtual bool initialize();
    virtual QString toolTip() const;
    virtual bool execute(const Sww::Core::PlugBase *plug);
    static Sww::Core::Node* creator();

    virtual Imath::M44f makeViewMatrix()const;
    virtual Imath::M44f makeProjectionMatrix(float deviceAspect) const;

    virtual void operate(int dx, int dy, int viewportWidth, int viewportHeight, const QMouseEvent* event);

private:
    Sww::Core::V2fPlug* _viewVolumeSize;

    bool updateShape();

};

}//ns SwwGeom

#endif // SWW_ORTHCAMERA_H

