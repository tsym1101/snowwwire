#include "KnobMatrix44f.h"
#include "ui_KnobMatrix44f.h"

#include <Core/Node.h>

#include "Knob/CustomSpinBox.h"
#include "Core/PlugType.h"

#include <ImathVec.h>
#include "Knob/CustomSpinBox.h"

#include <QSpacerItem>

KnobMatrix44f::KnobMatrix44f(Sww::Core::PlugBase *plug, QWidget *parent):KnobObject(plug,parent),
    ui(new Ui::KnobMatrix44f)
{
    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::M44F,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::M44fPlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");

    for(int row = 0;row < 4;row++){
        for(int col = 0;col<4;col++)
        {
            _spinBoxes[row][col] = new CustomSpinBox(this);
            ui->gridLayoutMat->addWidget(_spinBoxes[row][col],row,col);
        }
    }

    if(plug->isOutput())setEnabled(false);

    //if(plug->isOutput())ui->groupBox->setEnabled(false);

    connect(_spinBoxes[0][0],SIGNAL(editingFinished()),this,SLOT(m00_editingFinished()));
    connect(_spinBoxes[0][1],SIGNAL(editingFinished()),this,SLOT(m01_editingFinished()));
    connect(_spinBoxes[0][2],SIGNAL(editingFinished()),this,SLOT(m02_editingFinished()));
    connect(_spinBoxes[0][3],SIGNAL(editingFinished()),this,SLOT(m03_editingFinished()));
    connect(_spinBoxes[1][0],SIGNAL(editingFinished()),this,SLOT(m10_editingFinished()));
    connect(_spinBoxes[1][1],SIGNAL(editingFinished()),this,SLOT(m11_editingFinished()));
    connect(_spinBoxes[1][2],SIGNAL(editingFinished()),this,SLOT(m12_editingFinished()));
    connect(_spinBoxes[1][3],SIGNAL(editingFinished()),this,SLOT(m13_editingFinished()));
    connect(_spinBoxes[2][0],SIGNAL(editingFinished()),this,SLOT(m20_editingFinished()));
    connect(_spinBoxes[2][1],SIGNAL(editingFinished()),this,SLOT(m21_editingFinished()));
    connect(_spinBoxes[2][2],SIGNAL(editingFinished()),this,SLOT(m22_editingFinished()));
    connect(_spinBoxes[2][3],SIGNAL(editingFinished()),this,SLOT(m23_editingFinished()));
    connect(_spinBoxes[3][0],SIGNAL(editingFinished()),this,SLOT(m30_editingFinished()));
    connect(_spinBoxes[3][1],SIGNAL(editingFinished()),this,SLOT(m31_editingFinished()));
    connect(_spinBoxes[3][2],SIGNAL(editingFinished()),this,SLOT(m32_editingFinished()));
    connect(_spinBoxes[3][3],SIGNAL(editingFinished()),this,SLOT(m33_editingFinished()));
}

KnobMatrix44f::~KnobMatrix44f()
{
    delete ui;
}

bool KnobMatrix44f::initialize()
{
    if(_plug->prevCount())
    {
        setEnabled(false);
    }

    //ui->groupBox->setTitle(_plug->getName().c_str());

    Imath::M44f value;
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        const Imath::M44f& valueTmp = _plug->getConstInputValue(&ok);
        if(!ok){
            SWW_ERR("getInputValue() failed." << _plug->getDebugName());
            return false;
        }
        value = valueTmp;
    }

    for(int row = 0;row < 4;row++){
        for(int col = 0;col<4;col++)
        {
            _spinBoxes[row][col]->setValue(value.x[row][col]);
        }
    }

    return true;
}

KnobObject* KnobMatrix44f::creator(Sww::Core::PlugBase *plug , QWidget *parent)
{
    return new KnobMatrix44f(plug,parent);
}

void KnobMatrix44f::setFromSpinbox(int row,int col)
{
//    float f = _spinBoxes[row][col]->value();

//    Q_ASSERT(!isOutput());

//    bool ok;
//    Imath::M44f value  = _plug->getConstInputValue(&ok);
//    if(!ok){
//        SWW_ERR("getInputValue() failed." << _plug->getDebugName());
//        return;
//    }

//    ((float*)&value)[row * 4 + col] = f;

//    if(_plug->setInputValue(value) == false)
//    {
//        SWW_ERR("setFromSpinbox failed. " << _plug->getDebugName());
//    }

    float f = _spinBoxes[row][col]->value();
    Q_ASSERT(!isOutput());

    SWW_VAL_VOID(_plug,value);
    ((float*)&value)[row * 4 + col] = f;

    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    if(_plug->setInputValue(value) == false)
    {
        SWW_ERR("setFromSpinbox failed. " << _plug->getDebugName());
    }

    cmd->push();

}


void KnobMatrix44f::updateKnob()
{  
    if(_plug->prevCount() || _plug->isOutput())
    {
        setEnabled(false);
    }else  {
        setEnabled(true);
    }

    Imath::M44f value;
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        const Imath::M44f& valueTmp = _plug->getConstInputValue(&ok);
        if(!ok){
            SWW_ERR("postChangeState failed. " << _plug->getDebugName());
            return;
        }
        value = valueTmp;
    }

    for(int row = 0;row < 4;row++){
        for(int col = 0;col<4;col++)
        {
            _spinBoxes[row][col]->setValue(value.x[row][col]);
        }
    }

}

