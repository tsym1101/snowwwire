#ifndef SWW_CAMERA_H
#define SWW_CAMERA_H

#include "Geom/Shape.h"
#include "Core/SwwMacro.h"
#include "Core/Node.h"
#include "Geom/Lines.h"
#include "Geom/GeomAPI.h"

class QMouseEvent;

namespace Sww::Core{
class FloatPlug;
class V3fPlug;
class M44fPlug;
class EnumPlug;
class BoolPlug;
}

namespace SwwGeom {

class CameraPlug;
class LinesPlug;

#define SWW_VAL_M44F(plugPtr,result) SWW_VAL_RETURN(plugPtr,result,Imath::M44f())

class SWWGEOMSHARED_EXPORT Camera : public Sww::Core::Node
{
public:
    Camera();

    virtual bool initialize();
    static Sww::Core::Node* creator();
    virtual bool execute(const Sww::Core::PlugBase *plug);

    virtual Imath::M44f makeViewMatrix()const;
    virtual Imath::M44f makeViewMatrixTmp()const;
    virtual Imath::M44f makeProjectionMatrix(float deviceAspect)const; //eg.  deviceAspect =(double)viewportWidth/(double)viewportHeight;

    virtual void operate(int dx, int dy, int viewportWidth, int viewportHeight, const QMouseEvent* event);
    virtual void fit(const Imath::Box3f &bounds );

    void track(const Imath::V2f &point , int viewportWidth, int viewportHeight);
    void dolly(const Imath::V2f &point, int viewportWidth, float dollySpeed = 5.0f );
    void orbit(const Imath::V2f &point, int viewportWidth, int viewportHeight, float rotateSpeed = 400.0f );
    void lookAt( const Imath::V3f &eye, const Imath::V3f &at );

    float calcFovY(float deviceAspect) const;
    bool calcFov(Imath::V2f& value,float deviceAspect) const;

    bool getTransform(Imath::M44f &t);
    bool getV(Imath::M44f &m);

    bool TRisConnected()const;

    bool getTranslate(Imath::V3f& value)const;
    bool getRotate(Imath::V3f &value)const;
    bool getScale(Imath::V3f &value)const;
    bool getFocalLength(float& value)const;
    bool getNearClip(float& value)const;
    bool getFarClip(float& value)const;
    bool getAperture(Imath::V2f& value)const;

    bool getInheritedTranslate(Imath::V3f& value);
    bool getInheritedRotate(Imath::V3f& value);

    //Lines::ConstPtr getShape(bool &ok);

protected:
    Sww::Core::V3fPlug* _translate;
    Sww::Core::V3fPlug* _rotate;
    Sww::Core::V3fPlug* _scale;
    Sww::Core::M44fPlug* _iTransform;//parent transform.
    Sww::Core::M44fPlug* _oTransform;
    Sww::Core::M44fPlug* _V;

    CameraPlug* _camera;
    Sww::Core::FloatPlug* _nearClipPlug;
    Sww::Core::FloatPlug* _farClipPlug;
    Sww::Core::FloatPlug* _focalLengthPlug;//mm
    Sww::Core::FloatPlug* _horizontalAperturePlug;//cm
    Sww::Core::FloatPlug* _verticalAperturePlug;//cm
    Sww::Core::FloatPlug* _centerOfInterestPlug;
    Sww::Core::EnumPlug* _fitResolutionGatePlug;

    Sww::Core::BoolPlug* _lockMouseOperation;

    bool applyTransform(Imath::M44f& target,
                        Imath::V3f trans,Imath::V3f rotate,Imath::V3f scale);

    LinesPlug* _oShape;
    SwwGeom::Lines::Ptr _shapePtr;

    bool updateShape();

};

}

#endif // SWW_CAMERA_H
