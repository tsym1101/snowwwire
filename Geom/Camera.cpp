#include "Geom/Camera.h"
#include "Util/SwwMath.h"

#include "Core/PlugType.h"
#include "Geom/PlugTypeGeom.h"

#include <QMouseEvent>

#include <ImathMatrixAlgo.h>

#ifndef DEG_TO_RAD
#define DEG_TO_RAD 0.01745461
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG 57.2957796
#endif

static inline void rotateVector( double rx, double ry, Imath::V3f &v )
{
    rx =  rx * DEG_TO_RAD;

    const double sinX = sinf( rx );
    const double cosX = cosf( rx );

    const Imath::V3d t( v.x,
                        ( v.y * cosX ) - ( v.z * sinX ),
                        ( v.y * sinX ) + ( v.z * cosX ) );

    ry =  ry * DEG_TO_RAD;
    const double sinY = sinf( ry );
    const double cosY = cosf( ry );


    v.x = ( t.x * cosY ) + ( t.z * sinY );
    v.y = t.y;
    v.z = ( t.x * -sinY ) + ( t.z * cosY );
}

namespace SwwGeom {

Camera::Camera()
{
}

bool Camera::initialize(){

    _iTransform = new Sww::Core::M44fPlug("iTransform");
    _iTransform->setConnectable(true);
    _iTransform->setStorable(true);

    _translate = new Sww::Core::V3fPlug("translate");
    _rotate = new Sww::Core::V3fPlug("rotate");
    _scale = new Sww::Core::V3fPlug("scale",Imath::V3f(1.f,1.f,1.f));

    _translate->setConnectable(true);
    _rotate->setConnectable(true);
    _scale->setConnectable(true);

    _translate->setStorable(true);
    _rotate->setStorable(true);
    _scale->setStorable(true);

    _oTransform = new Sww::Core::M44fPlug("oTransform");
    _oTransform->setConnectable(true);
    _oTransform->setOutput(true);

    _V = new Sww::Core::M44fPlug("V");
    _V->setConnectable(true);
    _V->setOutput(true);

    _camera = new CameraPlug("camera");
    _camera->setOutput(true);
    _camera->setConnectable(true);

    _nearClipPlug = new Sww::Core::FloatPlug("nearClip",0.01f);
    _nearClipPlug->setConnectable(true);
    _nearClipPlug->setStorable(true);

    _farClipPlug = new Sww::Core::FloatPlug("farClip",1000.0f);
    _farClipPlug->setConnectable(true);
    _farClipPlug->setStorable(true);

    _focalLengthPlug = new Sww::Core::FloatPlug("focalLength",30.f);
    _focalLengthPlug->setConnectable(true);
    _focalLengthPlug->setStorable(true);

    _horizontalAperturePlug = new Sww::Core::FloatPlug("horizontalAperture",2.07264); //cm
    _horizontalAperturePlug->setStorable(true);

    _verticalAperturePlug = new Sww::Core::FloatPlug("verticalAperture",1.16586);//cm
    _verticalAperturePlug->setStorable(true);

    _centerOfInterestPlug = new Sww::Core::FloatPlug("centerOfInterest",15.0f);
    _centerOfInterestPlug->setStorable(true);
    _centerOfInterestPlug->setHidden(true);
    _centerOfInterestPlug->setConnectable(false);

    _fitResolutionGatePlug = new Sww::Core::EnumPlug("fitResolutionGate",0);
    _fitResolutionGatePlug->setStorable(true);
    *_fitResolutionGatePlug << "fill" << "vertical";

    _oShape = new SwwGeom::LinesPlug("oShape");
    _oShape->setOutput(true);
    _oShape->setConnectable(true);

    _lockMouseOperation = new Sww::Core::BoolPlug("lockMouseOperation",false);
    _lockMouseOperation->setStorable(true);

    addPlug(_iTransform);
    addPlug(_translate);
    addPlug(_rotate);
    addPlug(_scale);
    addPlug(_oTransform);
    addPlug(_V);

    addPlug(_camera);
    addPlug(_nearClipPlug);
    addPlug(_farClipPlug);
    addPlug(_focalLengthPlug);
    addPlug(_horizontalAperturePlug);
    addPlug(_verticalAperturePlug);
    addPlug(_centerOfInterestPlug);
    addPlug(_fitResolutionGatePlug);
    addPlug(_oShape);
    addPlug(_lockMouseOperation);

    _iTransform->affects(_camera);
    _translate->affects(_camera);
    _rotate->affects(_camera);
    _scale->affects(_camera);

    _iTransform->affects(_oTransform);
    _translate->affects(_oTransform);
    _rotate->affects(_oTransform);
    _scale->affects(_oTransform);

    _iTransform->affects(_V);
    _translate->affects(_V);
    _rotate->affects(_V);
    _scale->affects(_V);

    _nearClipPlug->affects(_camera);
    _farClipPlug->affects(_camera);
    _focalLengthPlug->affects(_camera);
    _horizontalAperturePlug->affects(_camera);
    _verticalAperturePlug->affects(_camera);
    _fitResolutionGatePlug->affects(_camera);

    _iTransform->affects(_oShape);
    _translate->affects(_oShape);
    _rotate->affects(_oShape);
    _scale->affects(_oShape);
    _focalLengthPlug->affects(_oShape);
    _horizontalAperturePlug->affects(_oShape);
    _verticalAperturePlug->affects(_oShape);

    lookAt(Imath::V3f(0.f,0.f,1.f),Imath::V3f(0.f));

    _camera->setOutputValue(this);

    return true;
}

Sww::Core::Node* Camera::creator(){
    return new Camera();
}

bool Camera::updateShape(){
    Sww::Core::V3fArrayPtr linesVertices(new Sww::Core::V3fArray(10));
    Sww::Core::IntArrayPtr counts(new Sww::Core::IntArray(2));

    SWW_VAL(_focalLengthPlug,focalLength);
    SWW_VAL(_verticalAperturePlug,verticalAperture);
    SWW_VAL(_horizontalAperturePlug,horizontalAperture);

    float fovyHalf = atan2(verticalAperture / 2.0,focalLength / 10.0); //ラジアン
    float fovxHalf = atan2(horizontalAperture / 2.0,focalLength / 10.0); //ラジアン

    float px = 3 * tan(fovxHalf);
    float py = 3 * tan(fovyHalf);
    float pz = 3;

    (*linesVertices)[0] = Imath::V3f(-px,py,-pz);
    (*linesVertices)[1] = Imath::V3f(-px,-py,-pz);
    (*linesVertices)[2] = Imath::V3f(px,-py,-pz);
    (*linesVertices)[3] = Imath::V3f(px,py,-pz);
    (*linesVertices)[4] = Imath::V3f(-px,py,-pz);
    (*linesVertices)[5] = Imath::V3f(0.0,0.0,0.0);
    (*linesVertices)[6] = Imath::V3f(px,py,-pz);

    (*linesVertices)[7] = Imath::V3f(-px,-py,-pz);
    (*linesVertices)[8] = Imath::V3f(0.0,0.0,0.0);
    (*linesVertices)[9] = Imath::V3f(px,-py,-pz);

    (*counts)[0] = (7);
    (*counts)[1] = (3);

    _shapePtr->setVertices(linesVertices);
    _shapePtr->setCounts(counts);
}

bool Camera::execute(const Sww::Core::PlugBase *plug)
{
    if(plug == _oShape){

        if(!_shapePtr){
            _shapePtr.reset(new SwwGeom::Lines(getName()+ "_cameraShape"));
            SWW_CHECK_BOOL(updateShape());
            _oShape->setOutputValue(_shapePtr);
        }

        Imath::M44f transform;
        SWW_CHECK_BOOL(getTransform(transform));
        _shapePtr->setTransform(transform);

        if(!_focalLengthPlug->isClean() || !_horizontalAperturePlug->isClean() || ! _verticalAperturePlug->isClean()){
            SWW_CHECK_BOOL(updateShape());
        }
    }
    else if(plug == _oTransform){
        Imath::M44f m;
        SWW_CHECK_BOOL(getTransform(m));
    }

    return true;
}

void Camera::lookAt( const Imath::V3f &eye, const Imath::V3f &at )
{
    _translate->setInputValue(eye);
    const Imath::V3f dt = at - eye;
    const double xzLen = sqrt( ( dt.x * dt.x ) +
                               ( dt.z * dt.z ) );

    bool ok;
    const Imath::V3f& iRotate = _rotate->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("lookat failed. " << getName() );
        return;
    }

    Imath::V3f oRotate(atan2( dt.y, float(xzLen) ) * RAD_TO_DEG,
                       atan2( dt.x, -dt.z )  * RAD_TO_DEG,
                       iRotate.z);
    _rotate->setInputValue(oRotate);
    _centerOfInterestPlug->setInputValue(dt.length());

    _translate->emitPlugStateChanged();
    _rotate->emitPlugStateChanged();

}

void Camera::track(const Imath::V2f &point,int viewportWidth,int viewportHeight)
{
    bool ok;
    const Imath::V3f& rotate = _rotate->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("track failed. " << getName() );
        return;
    }
    float centerOfInterest =_centerOfInterestPlug->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("track failed. " << getName() );
        return;
    }

    const float rotX = rotate.x;
    const float rotY = rotate.y;

    Imath::V3f dS( 1.0, 0.0, 0.0 );
    rotateVector( rotX, rotY, dS );

    Imath::V3f dT( 0.0, 1.0, 0.0 );
    rotateVector( rotX, rotY, dT );


    float fovy = calcFovY((double)viewportWidth/(double)viewportHeight);
    float multS = 2.0 * centerOfInterest * tanf(fovy * DEG_TO_RAD / 2.0 );
    const float multT = multS / float(viewportHeight);
    multS /= float(viewportWidth);

    // TRACK
    const float s = -multS * point.x;
    const float t = multT * point.y;

    // ALTER
    const Imath::V3f& trans = _translate->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("track failed. " << getName() );
        return;
    }

    _translate->setInputValue(trans + ( s * dS ) + ( t * dT ));

    _translate->emitPlugStateChanged();
}

void Camera::dolly(const Imath::V2f &point,int viewportWidth,float dollySpeed)
{
    bool ok;
    const Imath::V3f& trans = _translate->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("dolly failed. " << getName() );
        return;
    }
    const Imath::V3f& rotate = _rotate->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("dolly failed. " << getName() );
        return;
    }
    float centerOfInterest =_centerOfInterestPlug->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("dolly failed. " << getName() );
        return;
    }

    const float rotX = rotate.x;
    const float rotY = rotate.y;
    const Imath::V3f &eye = trans;

    Imath::V3f v( 0.0, 0.0, - centerOfInterest );
    rotateVector( rotX, rotY, v );
    const Imath::V3f view = eye + v;
    v.normalize();

    // DOLLY
    const float t = point.x / float(viewportWidth);

    // Magic dolly function
    float dollyBy = 1.0 - expf( -dollySpeed * t );

    if (dollyBy > 1.0)
    {
        dollyBy = 1.0;
    }
    else if (dollyBy < -1.0)
    {
        dollyBy = -1.0;
    }

    dollyBy *= centerOfInterest;
    const Imath::V3f newEye = eye + ( dollyBy * v );

    // ALTER
    _translate->setInputValue(newEye);

    v = newEye - view;
    _centerOfInterestPlug->setInputValue(v.length());

    _translate->emitPlugStateChanged();
}

void Camera::orbit(const Imath::V2f &point,int viewportWidth,int viewportHeight,float rotateSpeed)
{
    bool ok;
    const Imath::V3f& trans = _translate->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("orbit failed. " << getName() );
        return;
    }
    const Imath::V3f& rotate = _rotate->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("orbit failed. " << getName() );
        return;
    }
    float centerOfInterest =_centerOfInterestPlug->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("orbit failed. " << getName() );
        return;
    }

    float rotX = rotate.x;
    float rotY = rotate.y;
    const float rotZ = rotate.z;
    Imath::V3f eye = trans;

    Imath::V3f v( 0.0, 0.0, - centerOfInterest );
    rotateVector( rotX, rotY, v );

    const Imath::V3f view = eye + v;

    // ROTATE
    rotY += -rotateSpeed * ( point.x / float( viewportWidth) );
    rotX += -rotateSpeed * ( point.y / float( viewportHeight) );

    v[0] = 0.0;
    v[1] = 0.0;
    v[2] = centerOfInterest;
    rotateVector( rotX, rotY, v );

    //const Imath::V3f newEye = view + v;

    // ALTER
    _translate->setInputValue(view + v);
    _rotate->setInputValue(Imath::V3f( rotX, rotY, rotZ ));

    _translate->emitPlugStateChanged();
    _rotate->emitPlugStateChanged();
}

void Camera::fit(const Imath::Box3f &bounds)
{
    float r = 0.5f * bounds.size().length();
    //float fovy = calcFovY((double)viewportWidth/(double)viewportHeight);
    float fovy = 30;
    float fovyr = DEG_TO_RAD * fovy;

    float g = ( 1.1f * r ) / sinf( fovyr * 0.5f );
    lookAt( bounds.center() + Imath::V3f( 0, 0, g ), bounds.center() );
}

bool Camera::TRisConnected()const{
    return _translate->prevCount() || _rotate->prevCount();
}

Imath::M44f Camera::makeViewMatrix()const
{
    //SWW_DEBUG("makeViewMatrix " << getName());

    Imath::M44f m;
    Imath::M44f tmp;
    m.makeIdentity();

    bool ok = true;
    const Imath::V3f& translate = _translate->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("getInputValue failed. " << _translate->getDebugName());
        return m;
    }
    const Imath::V3f& rotate = _rotate->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("getInputValue failed. " << _rotate->getDebugName());
        return m;
    }
    const Imath::V3f& scale = _scale->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("getInputValue failed. " << _scale->getDebugName());
        return m;
    }

    const Imath::M44f& parentMatrix = _iTransform->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("getInputValue failed. " << _iTransform->getDebugName());
        return m;
    }

    m = parentMatrix.inverse() * m;
    m.scale(Imath::V3f( 1.0 / scale[0],
            1.0 / scale[1],
            1.0 / scale[2] ) );

    tmp.setAxisAngle( Imath::V3f( 0.0, 0.0, 1.0 ), (float)( -rotate[2] * DEG_TO_RAD ));
    m =  tmp * m;
    tmp.setAxisAngle( Imath::V3f( 1.0, 0.0, 0.0 ), (float)(- rotate[0] * DEG_TO_RAD) );
    m =  tmp * m;
    tmp.setAxisAngle( Imath::V3f( 0.0, 1.0, 0.0 ), (float)( -rotate[1] * DEG_TO_RAD) );
    m =  tmp * m;

    m.translate( Imath::V3f( -translate[0],
                 -translate[1],
            -translate[2] ) );
    return m;

}

Imath::M44f Camera::makeViewMatrixTmp()const
{
    //SWW_DEBUG("makeViewMatrix " << getName());

    Imath::M44f m;
    Imath::M44f tmp;
    m.makeIdentity();

    bool ok = true;
    const Imath::V3f& translate = _translate->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("getInputValue failed. " << _translate->getDebugName());
        return m;
    }
    const Imath::V3f& rotate = _rotate->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("getInputValue failed. " << _rotate->getDebugName());
        return m;
    }
    const Imath::V3f& scale = _scale->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("getInputValue failed. " << _scale->getDebugName());
        return m;
    }

    const Imath::M44f& parentMatrix = _iTransform->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("getInputValue failed. " << _iTransform->getDebugName());
        return m;
    }

    m = parentMatrix.inverse() * m;
    m.scale(Imath::V3f( 1.0 / scale[0],
            1.0 / scale[1],
            1.0 / scale[2] ) );

    tmp.setAxisAngle( Imath::V3f( 0.0, 0.0, 1.0 ), (float)( -rotate[2] * DEG_TO_RAD ));
    m =  tmp * m;
    tmp.setAxisAngle( Imath::V3f( 1.0, 0.0, 0.0 ), (float)(- rotate[0] * DEG_TO_RAD) );
    m =  tmp * m;
    tmp.setAxisAngle( Imath::V3f( 0.0, 1.0, 0.0 ), (float)( -rotate[1] * DEG_TO_RAD) );
    m =  tmp * m;

    m.translate( Imath::V3f( -translate[0],
                 -translate[1],
            -translate[2] ) );

    return m;

}

Imath::M44f Camera::makeProjectionMatrix(float deviceAspect)const{

    bool ok;
    float nearClip = _nearClipPlug->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("makeProjectionMatrix failed. " << getName() );
        return Imath::M44f();
    }
    float   farClip = _farClipPlug->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("makeProjectionMatrix failed. " << getName() );
        return Imath::M44f();
    }

    float fovy = calcFovY(deviceAspect);

    double radian= fovy * DEG_TO_RAD;
    double t = (double)(1.0 / tan(radian/2));

    Imath::M44f result;
    result[0][0] = t / deviceAspect;
    result[1][1] = t;
    result[2][2] = (farClip + nearClip) / (nearClip - farClip);
    result[2][3] = -1;
    result[3][2] = (2 * farClip * nearClip) / (nearClip - farClip);
    result[3][3] = 0;



    return result;
}


void Camera::operate(int dx, int dy,int viewportWidth, int viewportHeight,const QMouseEvent *event)
{

    SWW_VAL_VOID(_lockMouseOperation,lockMouse);

    if(!lockMouse){
        if (event->buttons() & Qt::RightButton ||
                ((event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::ShiftModifier) &&  (event->modifiers() & Qt::ControlModifier)) )
        {
            dolly(Imath::V2f(dx,dy),viewportWidth);

        }
        else if (event->buttons() & Qt::MiddleButton ||
                 ((event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::ShiftModifier)))
        {
            track(Imath::V2f(dx,dy),viewportWidth,viewportHeight);
        }
        else if (event->buttons() & Qt::LeftButton) {
            orbit(Imath::V2f(dx,dy),viewportWidth,viewportHeight);

        }
    }

    //SWW_DEBUG("camera operate.");
}


float Camera::calcFovY(float deviceAspect) const
{
    bool ok;
    float verticalAperture = _verticalAperturePlug->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("calcFovY failed. " << getName() );
        return 0;
    }

    float horizontalAperture = _horizontalAperturePlug->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("calcFovY failed. " << getName() );
        return 0;
    }

    float   focalLength = _focalLengthPlug->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("calcFovY failed. " << getName() );
        return 0;
    }

    int fitResolutionGate = _fitResolutionGatePlug->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("calcFovY failed. " << getName() );
        return 0;
    }

    if(fitResolutionGate == 0){//film gate fill.

        float filmAspect = horizontalAperture / verticalAperture;
        verticalAperture *= (filmAspect / deviceAspect);
    }
    else if(fitResolutionGate == 1){//film gate vertical.

    }

    else{
        Q_ASSERT(0);
    }


    float fovy = atan2(verticalAperture / 2.0,focalLength / 10.0) * RAD_TO_DEG * 2.0; //_focalLengthは単位mm


    return fovy;
}


bool Camera::calcFov(Imath::V2f &value, float deviceAspect) const{
    SWW_VAL(_verticalAperturePlug,verticalAperture);
    SWW_VAL(_horizontalAperturePlug,horizontalAperture);
    SWW_VAL(_focalLengthPlug,focalLength);
    SWW_VAL(_fitResolutionGatePlug,fitResolutionGate);

    if(fitResolutionGate == 0){//film gate fill.
        float filmAspect = horizontalAperture / verticalAperture;
        verticalAperture *= (filmAspect / deviceAspect);
    }
    else if(fitResolutionGate == 1){}//film gate vertical.

    float fovy = atan2(verticalAperture / 2.0,focalLength / 10.0) * RAD_TO_DEG * 2.0; //_focalLengthは単位mm
    float fovx = atan2(horizontalAperture / 2.0,focalLength / 10.0) * RAD_TO_DEG * 2.0; //_focalLengthは単位mm
    value = Imath::V2f(fovx,fovy);
    return true;
}

bool Camera::getTransform(Imath::M44f &t){
    if(_oTransform->isClean()){
        t = _oTransform->rawValue();
        return true;
    }

    SWW_VAL(_translate,trans);
    SWW_VAL(_rotate,rotate);
    SWW_VAL(_scale,scale);

    SWW_CHECK_BOOL(applyTransform(t,trans,rotate,scale));

    _oTransform->setOutputValue(t);
    _oTransform->setClean();//must for pushDirty()

    return true;
}

bool Camera::getV(Imath::M44f &m){

    if(_V->isClean()){
        m = _V->rawValue();
        return true;
    }

    m = makeViewMatrix();

    _V->setOutputValue(m);
    _V->setClean();//must for pushDirty()

    return true;
}


bool Camera::applyTransform(Imath::M44f &target,
                            Imath::V3f trans, Imath::V3f rotate, Imath::V3f scale){
    target.translate(trans);
    Imath::V3d rotTmp(Sww::Util::SwwMath::degToRad(rotate.x),
                      Sww::Util::SwwMath::degToRad(rotate.y),
                      Sww::Util::SwwMath::degToRad(rotate.z));
    target.rotate(rotTmp);
    target.scale(scale);

    SWW_VAL(_iTransform,parent);
    target = target * parent;

    return true;
}

bool Camera::getInheritedTranslate(Imath::V3f &value){
    SWW_VAL(_translate,trans);
    Imath::M44f m;
    SWW_CHECK_BOOL(getTransform(m));

    value.x = m[3][0];
    value.y = m[3][1];
    value.z = m[3][2];

//    SWW_DEBUG("getInheritedTranslate *************");
//    SWW_DEBUG_VALUE(trans);
//    SWW_DEBUG_VALUE(value);


    return true;
}

bool Camera::getInheritedRotate(Imath::V3f &value){
    SWW_VAL(_rotate,rot);
    Imath::M44f m;
    SWW_CHECK_BOOL(getTransform(m));

    Imath::V3f s,h,r,t;

    Imath::extractSHRT(m,s,h,r,t);//IMATH_INTERNAL_NAMESPACE::Euler<T>::XYZ

    value = Imath::V3f(Sww::Util::SwwMath::radToDeg(r.x),
                       Sww::Util::SwwMath::radToDeg(r.y),
                       Sww::Util::SwwMath::radToDeg(r.z));

    return true;
}

bool Camera::getTranslate(Imath::V3f &value)const{
    SWW_VAL(_translate,trans);
    value = trans;
    return true;
}
bool Camera::getRotate(Imath::V3f &value)const{
    SWW_VAL(_rotate,rotate);
    value  = rotate;
    return true;
}
bool Camera::getScale(Imath::V3f &value)const{
    SWW_VAL(_scale,scale);
    value = scale;
    return true;
}
bool Camera::getFocalLength(float &value)const{
    SWW_VAL(_focalLengthPlug,focalLength);
    value = focalLength;
    return true;
}
bool Camera::getNearClip(float &value)const{
    SWW_VAL(_nearClipPlug,nearClip);
    value = nearClip;
    return true;
}
bool Camera::getFarClip(float &value)const{
    SWW_VAL(_farClipPlug,farClip);
    value = farClip;
    return true;
}

bool Camera::getAperture(Imath::V2f &value) const{
    SWW_VAL(_horizontalAperturePlug,h);
    SWW_VAL(_verticalAperturePlug,v);

    value = Imath::V2f(h,v);
    return true;
}

//SwwGeom::Lines::ConstPtr Camera::getShape(bool& ok){
//    if(!_shapePtr){
//        _shapePtr.reset(new SwwGeom::Lines(getName()+ "_cameraShape"));

//        Sww::Core::V3fArrayPtr linesVertices(new Sww::Core::V3fArray());
//        Sww::Core::IntArrayPtr counts(new Sww::Core::IntArray());

//        linesVertices->push_back(Imath::V3f(-0.5,0.0,0.0));
//        linesVertices->push_back(Imath::V3f(0.5,0.0,0.0));
//        linesVertices->push_back(Imath::V3f(0.0,0.0,-0.5));
//        linesVertices->push_back(Imath::V3f(0.0,0.0,0.5));
//        linesVertices->push_back(Imath::V3f(0.0,-0.5,0.0));
//        linesVertices->push_back(Imath::V3f(0.0,0.5,0.0));
//        counts->push_back(2);
//        counts->push_back(2);
//        counts->push_back(2);

//        _shapePtr->setVertices(linesVertices);
//        _shapePtr->setCounts(counts);
//    }

//    Imath::M44f transform;
//    if(!getTransform(transform)){
//        ok = false;
//        return SwwGeom::Lines::ConstPtr();
//    }

//    _shapePtr->setTransform(transform);

//    ok = true;

//    return _shapePtr;

//}

}//ns
