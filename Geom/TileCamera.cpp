#include "TileCamera.h"
#include "Core/PlugType.h"
#include <glm/gtc/matrix_transform.hpp>
#include <PlugTypeGeom.h>
#include <QMouseEvent>

#ifndef DEG_TO_RAD
#define DEG_TO_RAD 0.01745461
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG 57.2957796
#endif


namespace  SwwGeom{

TileCamera::TileCamera()
{
}

bool TileCamera::initialize()
{
    SWW_CHECK_BOOL(Camera::initialize());

    _interaxial = new Sww::Core::FloatPlug("interaxial",0.01f);
    _interaxial->setConnectable(true);
    _interaxial->setStorable(true);

    _convergence = new Sww::Core::FloatPlug("convergence",1.0f);
    _convergence->setConnectable(true);
    _convergence->setStorable(true);

    _tileDim = new Sww::Core::V2iPlug("tileDim",Imath::V2i(2,1));
    _tileDim->setStorable(true);
    _tileDim->setConnectable(true);

    _numTile = new Sww::Core::IntPlug("numTile",2);
    _numTile->setConnectable(true);
    _numTile->setStorable(true);

    _tileOffset = new Sww::Core::IntPlug("tileOffset");
    _tileOffset->setConnectable(true);
    _tileOffset->setStorable(true);

    _inverseCameraIndex = new Sww::Core::BoolPlug("inverseCameraIndex",false);
    _inverseCameraIndex->setStorable(true);

    _orthoView = new Sww::Core::BoolPlug("orthoView",true);
    _orthoView->setStorable(true);

    _orthoViewVolumeSize = new Sww::Core::V2fPlug("orthoViewVolumeSize",Imath::V2f(2.0,2.0));
    _orthoViewVolumeSize->setConnectable(true);
    _orthoViewVolumeSize->setStorable(true);

    _flipTileY = new Sww::Core::BoolPlug("flipTileY",false);
    _flipTileY->setStorable(true);

    addPlug(_interaxial);
    addPlug(_convergence);
    addPlug(_tileDim);
    addPlug(_numTile);
    addPlug(_tileOffset);
    addPlug(_inverseCameraIndex);
    addPlug(_orthoView);
    addPlug(_orthoViewVolumeSize);
    addPlug(_flipTileY);

    _interaxial->affects(_camera);
    _convergence->affects(_camera);
    _tileDim->affects(_camera);
    _numTile->affects(_camera);
    _tileOffset->affects(_camera);
    _inverseCameraIndex->affects(_camera);
    _orthoView->affects(_camera);
    _orthoViewVolumeSize->affects(_camera);
    _flipTileY->affects(_camera);

    _interaxial->affects(_oShape);
    _convergence->affects(_oShape);
    _numTile->affects(_oShape);
    _inverseCameraIndex->affects(_oShape);
    _orthoView->affects(_oShape);
    _orthoViewVolumeSize->affects(_oShape);

    return true;
}
QString TileCamera::toolTip()const
{
    QString str = "your tooltip";
    return str;
}

Sww::Core::Node* TileCamera::creator(){
    return new TileCamera();
}

bool TileCamera::updateShape(){

    Sww::Core::V3fArrayPtr linesVertices(new Sww::Core::V3fArray());
    Sww::Core::IntArrayPtr counts(new Sww::Core::IntArray());

    SWW_VAL(_orthoView,orthoView);

    SWW_VAL(_interaxial,interaxial);
    SWW_VAL(_convergence,convergence);
    SWW_VAL(_numTile,numTile);
    SWW_VAL(_inverseCameraIndex,inverseCameraIndex);

    if(orthoView){
        SWW_VAL(_orthoViewVolumeSize,orthoViewVolumeSize);

        float px = orthoViewVolumeSize.x / 2.0;
        float py = orthoViewVolumeSize.y / 2.0;
        float pz = -convergence * 2.0;

        for(int i = 0;i < numTile;i++){

            int n = i;
            if(inverseCameraIndex){
                n = numTile - 1 - n;
            }
            int shiftStep  = -2 * n + (numTile -1);
            shiftStep *= -1;//projectionMatrixを作るときとは逆

            float shift = shiftStep * interaxial * 0.5;

            linesVertices->push_back(Imath::V3f(-px + shift,py,0));
            linesVertices->push_back(Imath::V3f(-px + shift,-py,0));
            linesVertices->push_back(Imath::V3f(px + shift,-py,0));
            linesVertices->push_back(Imath::V3f(px + shift,py,0));
            linesVertices->push_back(Imath::V3f(-px + shift,py,0));
            linesVertices->push_back(Imath::V3f(-px - shift,py,pz));
            linesVertices->push_back(Imath::V3f(-px - shift,-py,pz));
            linesVertices->push_back(Imath::V3f(px - shift,-py,pz));
            linesVertices->push_back(Imath::V3f(px - shift,py,pz));
            linesVertices->push_back(Imath::V3f(-px - shift,py,pz));

            linesVertices->push_back(Imath::V3f(-px + shift,-py,0));
            linesVertices->push_back(Imath::V3f(-px - shift,-py,pz));

            linesVertices->push_back(Imath::V3f(px + shift,-py,0));
            linesVertices->push_back(Imath::V3f(px - shift,-py,pz));

            linesVertices->push_back(Imath::V3f(px + shift,py,0));
            linesVertices->push_back(Imath::V3f(px - shift,py,pz));

            counts->push_back(10);
            counts->push_back(2);
            counts->push_back(2);
            counts->push_back(2);

            //cross
            linesVertices->push_back(Imath::V3f(-0.5 + shift,0.0,0.0));
            linesVertices->push_back(Imath::V3f(0.5 + shift,0.0,0.0));
            linesVertices->push_back(Imath::V3f(0.0 + shift,-0.5,0.0));
            linesVertices->push_back(Imath::V3f(0.0 + shift,0.5,0.0));

            counts->push_back(2);
            counts->push_back(2);
        }
    }else{
        SWW_VAL(_focalLengthPlug,focalLength);
        SWW_VAL(_verticalAperturePlug,verticalAperture);
        SWW_VAL(_horizontalAperturePlug,horizontalAperture);

        float fovyHalf = atan2(verticalAperture / 2.0,focalLength / 10.0); //ラジアン
        float fovxHalf = atan2(horizontalAperture / 2.0,focalLength / 10.0); //ラジアン

        float px = convergence * 2 * tan(fovxHalf);
        float py = convergence * 2 * tan(fovyHalf);
        float pz = convergence * 2;

        for(int i = 0;i < numTile;i++){

            int n = i;
            if(inverseCameraIndex){
                n = numTile - 1 - n;
            }
            int shiftStep  = -2 * n + (numTile -1);
            shiftStep *= -1;//projectionMatrixを作るときとは逆

            float shift = shiftStep * interaxial * 0.5;

            linesVertices->push_back(Imath::V3f(-px - shift,py,-pz));
            linesVertices->push_back(Imath::V3f(-px - shift,-py,-pz));
            linesVertices->push_back(Imath::V3f(px - shift,-py,-pz));
            linesVertices->push_back(Imath::V3f(px - shift,py,-pz));
            linesVertices->push_back(Imath::V3f(-px - shift,py,-pz));
            linesVertices->push_back(Imath::V3f(shift,0.0,0.0));
            linesVertices->push_back(Imath::V3f(px - shift,py,-pz));

            linesVertices->push_back(Imath::V3f(-px - shift,-py,-pz));
            linesVertices->push_back(Imath::V3f(shift,0.0,0.0));
            linesVertices->push_back(Imath::V3f(px - shift,-py,-pz));

            counts->push_back(7);
            counts->push_back(3);
        }
    }

    _shapePtr->setVertices(linesVertices);
    _shapePtr->setCounts(counts);

    return true;
}

bool TileCamera::execute(const Sww::Core::PlugBase *plug)
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

        if(!_interaxial->isClean() ||
                !_convergence->isClean() ||
                !_numTile->isClean() ||
                !_inverseCameraIndex->isClean() ||
                !_orthoView->isClean() ||
                !_orthoViewVolumeSize->isClean() ){
            SWW_CHECK_BOOL(updateShape());
        }
    }
    else if(plug == _oTransform){
        Imath::M44f m;
        SWW_CHECK_BOOL(getTransform(m));
    }

    return true;
}

bool TileCamera::makeViewProjectionMatrix(Imath::M44f& viewMatrix, Imath::M44f& projectionMatrix,
                                          Imath::V4f &viewport,
                                          int viewportWidth, int viewportHeight ,
                                          int tileStepX, int tileStepY) const
{
    SWW_VAL(_orthoView,orthView);

    if(orthView){
        return makeViewProjectionMatrixOrtho(viewMatrix,projectionMatrix,
                                             viewport,
                                             viewportWidth, viewportHeight ,
                                             tileStepX,tileStepY);
    }else{
        return makeViewProjectionMatrixOffAxis(viewMatrix,projectionMatrix,
                                               viewport,
                                               viewportWidth, viewportHeight ,
                                               tileStepX,tileStepY);
    }
}

//Imath::M44f TileCamera::makeProjectionMatrix(float deviceAspect)const //引数使わない
//{
//    SWW_VAL_M44F(_orthoView,orthView);

//    if(orthView){
//        return makeProjectionMatrixOrth();
//    }else{
//        Q_ASSERT(0);
//    }

//}


bool TileCamera::makeViewProjectionMatrixOrtho(Imath::M44f &viewMatrix, Imath::M44f &projectionMatrix,
                                               Imath::V4f &viewport,
                                               int viewportWidth, int viewportHeight ,
                                               int tileStepX, int tileStepY) const
{
    SWW_VAL(_interaxial,interaxial);
    SWW_VAL(_convergence,convergence);
    SWW_VAL(_tileDim,tileDim);
    SWW_VAL(_numTile,numTile);
    SWW_VAL(_inverseCameraIndex,inverseCameraIndex);
    SWW_VAL(_orthoViewVolumeSize,orthoViewVolumeSize);
    SWW_VAL(_nearClipPlug,nearClip);
    SWW_VAL(_farClipPlug,farClip);
    SWW_VAL(_flipTileY,flipTileY);


    viewMatrix.makeIdentity();
    //コンバージェンスの位置に原点を移動
    viewMatrix.translate(Imath::V3f(0,0,(-convergence)));


    int n = tileStepY * tileDim.x + tileStepX;

    if(inverseCameraIndex){
        n = numTile - 1 - n;
    }

    //カメラは左が0
    //シフト量の乗算値 カメラ数に応じて変動 せん断行列に使うtanに乗算する
    int shiftStep  = -2 * n + (numTile -1);

    //せん断
    float tan = (interaxial/2.0f) / convergence;

    float m[16] = {1,0,0,0,
                   0,1,0,0,
                   shiftStep * tan,0,1,0,
                   0,0,0,1};

    Imath::M44f mTmp = *((Imath::M44f*)m);
    viewMatrix = mTmp * viewMatrix;

    //原点を元に戻す
    viewMatrix.translate(Imath::V3f(0,0,convergence));

    viewMatrix = Camera::makeViewMatrix() * viewMatrix;

    glm::mat4 projTmp = glm::ortho((float)(-orthoViewVolumeSize.x/2.0),
                                   (float)(orthoViewVolumeSize.x/2.0),
                                   (float)(-orthoViewVolumeSize.y/2.0),
                                   (float)(orthoViewVolumeSize.y/2.0),
                                   (float)(nearClip),
                                   (float)(farClip));

    projectionMatrix = *((Imath::M44f*)&projTmp);

    if(flipTileY){
        tileStepY = tileDim.y - tileStepY  - 1;
    }

    int xTmp = (viewportWidth/tileDim.x) * tileStepX;
    int yTmp = (viewportHeight/tileDim.y) * tileStepY;
    int wTmp = viewportWidth/tileDim.x;
    int hTmp = viewportHeight/tileDim.y;

    viewport.x = xTmp;
    viewport.y = yTmp;
    viewport.z = wTmp;
    viewport.w = hTmp;

    return true;
}

bool TileCamera::makeViewProjectionMatrixOffAxis(Imath::M44f &viewMatrix, Imath::M44f &projectionMatrix,
                                                 Imath::V4f &viewport,
                                                 int viewportWidth, int viewportHeight,
                                                 int tileStepX, int tileStepY)const
{
    SWW_VAL(_interaxial,interaxial);
    SWW_VAL(_convergence,convergence);
    SWW_VAL(_nearClipPlug,nearClip);
    SWW_VAL(_farClipPlug,farClip);
    SWW_VAL(_tileDim,tileDim);
    SWW_VAL(_numTile,numTile);
    SWW_VAL(_tileOffset,tileOffset);
    SWW_VAL(_inverseCameraIndex,inverseCameraIndex);

    tileOffset = tileOffset % numTile;
    if(tileOffset < 0)tileOffset = numTile + tileOffset;

    int tileStepXTmp =  (tileStepX + tileOffset) % tileDim.x;
    int tileStepYTmp = tileStepY +  (tileStepX + tileOffset) / tileDim.x;

    int n = tileStepYTmp * tileDim.x + tileStepXTmp - tileOffset;

    if(inverseCameraIndex){
        n = numTile - 1 - n;
    }
    //カメラは左が0
    int step  = -2 * n + (numTile -1);


    Frustum frustum = calcFrustumShift(viewportWidth,interaxial,convergence);
    float top,bottom,left,right;

    top = frustum.top;
    bottom = -frustum.top;

    left = -frustum.right + ((frustum.shift) * step);
    right = frustum.right + ((frustum.shift) * step);

    glm::mat4 projTmp = glm::frustum(left,right,bottom,top,
                                     nearClip,farClip);

    projectionMatrix = *((Imath::M44f*)&projTmp);

    float shiftX = (interaxial / 2.f) * step;//カメラ位置からのinteraxialシフト量
    viewMatrix.makeIdentity();
    viewMatrix.translate(Imath::V3f(shiftX,0,0));
    viewMatrix =  Camera::makeViewMatrix() * viewMatrix;

    int xTmp = (viewportWidth/tileDim.x) * tileStepX;
    int yTmp = (viewportHeight/tileDim.y) * tileStepY;
    int wTmp = viewportWidth/tileDim.x;
    int hTmp = viewportHeight/tileDim.y;

    viewport.x = xTmp;
    viewport.y = yTmp;
    viewport.z = wTmp;
    viewport.w = hTmp;

    return true;
}


Frustum TileCamera::calcFrustumShift(float deviceAspect ,float interaxial,float convergence)const
{
    bool ok;
    float nearClip = _nearClipPlug->getConstInputValue(&ok);
    if(!ok){
        SWW_ERR("invalid. " << getName() );
        return Frustum();
    }

    Frustum result;

    float fovy = calcFovY(deviceAspect);

    result.top = nearClip * tan( DEG_TO_RAD * fovy / 2.f);
    result.right = deviceAspect * result.top;
    result.shift = (interaxial / 2.f) * (nearClip) / convergence;
    return result;
}



void TileCamera::operate(int dx, int dy,int viewportWidth, int viewportHeight,const QMouseEvent *event)
{
    SWW_VAL_VOID(_lockMouseOperation,lockMouse);
    if(!lockMouse){
        if (event->modifiers() == Qt::AltModifier
                && event->buttons() & Qt::LeftButton)
        {
            orbit(Imath::V2f(dx,dy),viewportWidth,viewportHeight);
        }
        else if (event->modifiers() == Qt::AltModifier
                 && event->buttons() & Qt::RightButton)
        {
            dolly(Imath::V2f(dx,dy),viewportWidth);
        }
        else if (event->modifiers() == Qt::AltModifier
                 && event->buttons() & Qt::MiddleButton)
        {
            track(Imath::V2f(dx,dy),viewportWidth,viewportHeight);
        }
        else if(event->modifiers() == (Qt::ControlModifier|Qt::AltModifier)
                && event->buttons() & Qt::LeftButton)
        {

            bool ok;
            float interaxial = _interaxial->getConstInputValue(&ok);
            if(!ok){
                SWW_ERR("invalid. " << getName() );
                return;
            }

            float dInter = interaxial + (float)dx / (float)600;

            if (dInter >= 0)
            {
                _interaxial->setInputValue(dInter);
            }else
            {
                _interaxial->setInputValue(0);
            }

            _interaxial->emitPlugStateChanged();


        }
        else if (event->modifiers() == (Qt::ControlModifier|Qt::AltModifier)
                 && event->buttons() & Qt::RightButton)
        {

            bool ok;
            float convergence = _convergence->getConstInputValue(&ok);
            if(!ok){
                SWW_ERR("invalid. " << getName() );
                return;
            }

            convergence += (float)dx / 200.0;

            if(convergence <= 0)
            {
                convergence = 0.001f;
            }

            _convergence->setInputValue(convergence);

            _convergence->emitPlugStateChanged();

        }
        else if (event->modifiers() == (Qt::ControlModifier|Qt::AltModifier)
                 && event->buttons() & Qt::MiddleButton)
        {
            bool ok;
            Imath::V3f trans = _translate->getConstInputValue(&ok);
            if(!ok){
                SWW_ERR("invalid. " << getName() );
                return;
            }
            trans.z = trans.z + (float)dx / 600.0;
            _translate->setInputValue(trans);

            _translate->emitPlugStateChanged();
        }

    }
}


bool TileCamera::getTileDim(Imath::V2i& value)const{
    SWW_VAL(_tileDim,tileDim);
    value = tileDim;
    return true;
}

bool TileCamera::getNumTile(int &value) const{
    SWW_VAL(_numTile,numTile);
    value = numTile;
    return true;
}

void TileCamera::fit(const Imath::Box3f &bounds){

    bool ok;
    bool orthoView = _orthoView->getConstInputValue(&ok);
    if(!ok)return;

    if(orthoView){
        float r = 0.5f * bounds.size().length();
        float g = ( 1.1f * r );
        Imath::V3f size = bounds.size();
        Imath::V2f viewVolumeSize(size.x,size.y);
        _orthoViewVolumeSize->setInputValue(viewVolumeSize);
        _orthoViewVolumeSize->emitPlugStateChanged();

        lookAt( bounds.center() + Imath::V3f( 0, 0, g ), bounds.center() );
    }
    else{
        Camera::fit(bounds);
    }

}

}//ns SwwGeom

