#include "KnobMatrix33f.h"
#include "ui_KnobMatrix44f.h"

#include <Core/Node.h>

#include "Knob/CustomSpinBox.h"
#include "Core/PlugType.h"

#include <ImathVec.h>
#include "Knob/CustomSpinBox.h"

#include <QSpacerItem>

KnobMatrix33f::KnobMatrix33f(Sww::Core::PlugBase *plug, QWidget *parent):KnobObject(plug,parent),
    ui(new Ui::KnobMatrix44f)
{
    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::M33F,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::M33fPlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");

    for(int row = 0;row < 3;row++){
        for(int col = 0;col<3;col++)
        {
            _spinBoxes[row][col] = new CustomSpinBox(this);
            ui->gridLayoutMat->addWidget(_spinBoxes[row][col],row,col);
        }
    }

    if(plug->isOutput())setEnabled(false);

    connect(_spinBoxes[0][0],SIGNAL(editingFinished()),this,SLOT(m00_editingFinished()));
    connect(_spinBoxes[0][1],SIGNAL(editingFinished()),this,SLOT(m01_editingFinished()));
    connect(_spinBoxes[0][2],SIGNAL(editingFinished()),this,SLOT(m02_editingFinished()));
    connect(_spinBoxes[1][0],SIGNAL(editingFinished()),this,SLOT(m10_editingFinished()));
    connect(_spinBoxes[1][1],SIGNAL(editingFinished()),this,SLOT(m11_editingFinished()));
    connect(_spinBoxes[1][2],SIGNAL(editingFinished()),this,SLOT(m12_editingFinished()));
    connect(_spinBoxes[2][0],SIGNAL(editingFinished()),this,SLOT(m20_editingFinished()));
    connect(_spinBoxes[2][1],SIGNAL(editingFinished()),this,SLOT(m21_editingFinished()));
    connect(_spinBoxes[2][2],SIGNAL(editingFinished()),this,SLOT(m22_editingFinished()));
}

KnobMatrix33f::~KnobMatrix33f()
{
    delete ui;
}

bool KnobMatrix33f::initialize()
{
    if(_plug->prevCount())
    {
       setEnabled(false);
    }

    //ui->groupBox->setTitle(_plug->getName().c_str());

    Imath::M33f value;
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        const Imath::M33f& valueTmp = _plug->getConstInputValue(&ok);
        if(!ok){
            SWW_ERR("getInputValue() failed." << _plug->getDebugName());
            return false;
        }
        value = valueTmp;
    }

    for(int row = 0;row < 3;row++){
        for(int col = 0;col<3;col++)
        {
            _spinBoxes[row][col]->setValue(value.x[row][col]);
        }
    }

    return true;
}

KnobObject* KnobMatrix33f::creator(Sww::Core::PlugBase *plug , QWidget *parent)
{
    return new KnobMatrix33f(plug,parent);
}

void KnobMatrix33f::setFromSpinbox(int row,int col)
{
    float f = _spinBoxes[row][col]->value();
    Q_ASSERT(!isOutput());

    SWW_VAL_VOID(_plug,value);
    ((float*)&value)[row * 3 + col] = f;

    SwwKnob::UndoCommand* cmd = new SwwKnob::UndoCommand(_plug);
    cmd->storeUndoValue();

    if(_plug->setInputValue(value) == false)
    {
        SWW_ERR("setFromSpinbox failed. " << _plug->getDebugName());
    }

    cmd->push();
}


void KnobMatrix33f::updateKnob()
{  
    if(_plug->prevCount() || _plug->isOutput())
    {
        setEnabled(false);
    }else  {
        setEnabled(true);
    }

    Imath::M33f value;
    if(isOutput())
    {
        _plug->getOutputValue(value);
    }else
    {
        bool ok;
        const Imath::M33f& valueTmp = _plug->getConstInputValue(&ok);
        if(!ok){
            SWW_ERR("postChangeState failed. " << _plug->getDebugName());
            return;
        }
        value = valueTmp;
    }

    for(int row = 0;row < 3;row++){
        for(int col = 0;col<3;col++)
        {
            _spinBoxes[row][col]->setValue(value.x[row][col]);
        }
    }

}

