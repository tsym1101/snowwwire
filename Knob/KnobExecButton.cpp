#include "KnobExecButton.h"
#include "ui_KnobExecButton.h"

#include "Core/PlugType.h"

KnobExecButton::KnobExecButton(Sww::Core::PlugBase *plug, QWidget *parent) :
    KnobObject(plug,parent),
    ui(new Ui::KnobExecButton)
{
    ui->setupUi(this);

    Q_ASSERT(plug);
    Q_ASSERT_X(plug->getSwwTypeDesc() == Sww::Core::_NULL,plug->getDebugName().c_str(),"mismatch type.");
    _plug = dynamic_cast<Sww::Core::NullPlug*>(plug);
    Q_ASSERT_X(_plug,plug->getDebugName().c_str(),"dynamic cast failed.");
}

KnobExecButton::~KnobExecButton()
{
    delete ui;
}

KnobObject* KnobExecButton::creator(Sww::Core::PlugBase *plug, QWidget *parent)
{
    return new KnobExecButton(plug,parent);
}

bool KnobExecButton::initialize()
{
    ui->pushButtonExec->setText(_plug->getName().c_str());


//    QFont font = ui->pushButtonExec->font();
//    font.setPointSize(40);
//    ui->pushButtonExec->setFont(font);
    return true;
}

void KnobExecButton::on_pushButtonExec_clicked()
{
    Q_ASSERT(_plug->isOutput());
   _plug->pull(_plug->isLockedWhenPull());
}
