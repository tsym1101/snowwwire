//#include <GL/glew.h>

#include "KnobTF.h"
#include "ui_KnobTF.h"

#include "Geom/PlugTypeGeom.h"
#include "Knob/TransfuncMappingCanvas.h"
#include "Geom/TransFunc.h"
#include "Geom/TransFuncMappingKey.h"
#include "Geom/TransFuncIntensity.h"

//#include "OpenGL/GLTexture.h"
//#include "OpenGL/GLGlobal.h"

KnobTF::KnobTF(Sww::Core::PlugBase *plug, QWidget *parent) :
    KnobObject(plug,parent),
    ui(new Ui::KnobTF)
{
    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == SwwGeom::TRANSFERFUNCTION_DESC,
               plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<SwwGeom::TransferFunctionPlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");
    //    SWW_VAL_VOID(_plug,tf);
    //    SWW_DEBUG_VALUE(tf->getNumKeys());
}

KnobTF::~KnobTF()
{
    delete ui;
}

void KnobTF::setToGui(){
    SWW_VAL_VOID(_plug,tf);
    SWW_DEBUG_VALUE(tf->getNumKeys());


    int reso = tf->getDimensions().x;
    SWW_DEBUG_VALUE( reso);
    ui->spinBoxResolution->setValue(reso);

    //    GLenum target = _plug->getTF()->getTexTarget();

    //    switch (target) {
    //    case GL_TEXTURE_1D:
    //        ui->comboBoxMode->setCurrentIndex(0);
    //        break;
    //    case GL_TEXTURE_2D:
    //        ui->comboBoxMode->setCurrentIndex(1);
    //        break;
    //    default:
    //        Q_ASSERT_X(0,_plug->getDebugName().c_str(),"invalid texTarget.");
    //    }
}

bool KnobTF::initialize(){

    //    SWW_VAL(_plug,tf);
    //    Q_ASSERT(tf);

    _canvas = new SwwGeom::TransFuncMappingCanvas(this,_plug);

    if(_plug->prevCount())
    {
        _canvas->setEnabled(false);
    }else{
        _canvas->setEnabled(true);
    }

    ui->label->setText(_plug->getName().c_str());

    ui->verticalLayout_2->addWidget(_canvas);
    //ui->scrollArea->setWidget(_canvas);
    if(_plug->isOutput())_canvas->setEnabled(false);//outputは編集できない
    connect(_canvas,SIGNAL(changed()),this,SLOT(tf_modified()));
    connect(_canvas,SIGNAL(resetTransferFunction()),this,SLOT(tf_reset()));
    _canvas->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    setToGui();

    return true;
}

void KnobTF::updateKnob()
{
    if(_plug->prevCount())
    {
        _canvas->setEnabled(false);
    }else{
        _canvas->setEnabled(true);
    }

    SWW_VAL_VOID(_plug,tf);
    Q_ASSERT(tf);
    _canvas->setTransFunc(tf.get());
    setToGui();
}

KnobObject* KnobTF::creator(Sww::Core::PlugBase *plug ,QWidget *parent)
{
    return new KnobTF(plug,parent);
}

void KnobTF::tf_modified(){
    _plug->setClean();//これを忘れるとpushDirtyが止まる
    _plug->postSetRoutine();
    //_plug->pushDirty();

    //SWW_DEBUG_VALUE("tf changed");
}

void KnobTF::tf_reset(){
    SWW_VAL_VOID(_plug,tf);
    tf->createStdFunc();
    _plug->setClean();
    _plug->pushDirty();
}

void KnobTF::on_spinBoxResolution_editingFinished()
{
    int value = ui->spinBoxResolution->value();
    SWW_VAL_VOID(_plug,tf);
    tf->resize(value);
    _plug->setClean();
    _plug->pushDirty();

    SWW_DEBUG(tf->valueToString().toStdString());
}

//void KnobTF::on_comboBoxMode_activated(int index)
//{
////    SWW_DEBUG("combo");
////    GLenum target = GL_TEXTURE_1D;
////    switch (index) {
////    case 0:
////        target = GL_TEXTURE_1D;
////        break;
////    case 1:
////        target = GL_TEXTURE_2D;
////        break;
////    default:
////        return;
////    }

//    SWW_VAL_VOID(_plug,tf);
//    tf->setTexTarget(target);
//    _plug->setClean();
//    _plug->pushDirty();
//    //SWW_DEBUG("combo2");
//}
