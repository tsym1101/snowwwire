#ifndef SWW_TILECAMERA_H
#define SWW_TILECAMERA_H

#include "Geom/Camera.h"
#include "Core/PlugTypeDefFwd.h"

namespace SwwGeom {

struct Frustum{
    float top;
    float right;
    float shift;
};

class SWWGEOMSHARED_EXPORT TileCamera :public SwwGeom::Camera
{
public:
    TileCamera();
    virtual ~TileCamera(){}
    virtual bool initialize();
    virtual QString toolTip() const;
    virtual bool execute(const Sww::Core::PlugBase *plug);
    static Sww::Core::Node* creator();

    virtual bool makeViewProjectionMatrix(Imath::M44f& viewMatrix, Imath::M44f& projectionMatrix,
                                          Imath::V4f& viewport,
                                          int viewportWidth, int viewportHeight,
                                          int tileStepX, int tileStepY)const;

    Frustum calcFrustumShift(float deviceAspect ,float interaxial,float convergence)const;

    virtual void operate(int dx, int dy, int viewportWidth, int viewportHeight, const QMouseEvent* event);

    bool getTileDim(Imath::V2i& value)const;
    bool getNumTile(int& value)const;

    virtual void fit(const Imath::Box3f &bounds );

private:

    bool makeViewProjectionMatrixOrtho(Imath::M44f& viewMatrix, Imath::M44f& projectionMatrix,
                                       Imath::V4f &viewport,
                                       int viewportWidth, int viewportHeight ,
                                       int tileStepX, int tileStepY)const;
    bool makeViewProjectionMatrixOffAxis(Imath::M44f& viewMatrix,Imath::M44f& projectionMatrix,
                                         Imath::V4f &viewport,
                                         int viewportWidth, int viewportHeight ,
                                         int tileStepX, int tileStepY)const;

    Sww::Core::FloatPlug* _interaxial;
    Sww::Core::FloatPlug* _convergence;
    Sww::Core::V2iPlug* _tileDim;
    Sww::Core::IntPlug* _numTile;

    Sww::Core::IntPlug* _tileOffset;
    Sww::Core::BoolPlug* _inverseCameraIndex;
    Sww::Core::BoolPlug* _orthoView;
    Sww::Core::V2fPlug* _orthoViewVolumeSize;

    Sww::Core::BoolPlug* _flipTileY; //タイルの並びの縦を反転するか


    virtual Imath::M44f makeViewMatrix()const{
        return Camera::makeViewMatrix();
    }
    virtual Imath::M44f makeProjectionMatrix(float deviceAspect) const{
        return Camera::makeProjectionMatrix(deviceAspect);
    }

    bool updateShape();
};

}//ns SwwGeom

#endif // SWW_TILECAMERA_H
