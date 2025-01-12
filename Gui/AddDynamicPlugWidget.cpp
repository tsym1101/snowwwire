#include "AddDynamicPlugWidget.h"
#include "ui_AddDynamicPlugWidget.h"
#include "Core/Node.h"
#include "Core/PlugType.h"

#include "Core/PlugRegister.h"

AddDynamicPlugWidget::AddDynamicPlugWidget(Sww::Core::Node *node, QWidget *parent) :
    QWidget(parent),_node(node),
    ui(new Ui::AddDynamicPlugWidget)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    QStringList list = Sww::Core::PlugRegister::getAvailablePlugNames();
    ui->comboBoxPlugType->addItems(list);
}

AddDynamicPlugWidget::~AddDynamicPlugWidget()
{
    delete ui;
}

void AddDynamicPlugWidget::on_pushButtonOk_clicked()
{
    std::string plugName = ui->lineEditPlugName->text().toStdString();
    std::string plugTypeName = ui->comboBoxPlugType->currentText().toStdString();
    bool isOutput = ui->checkBoxASOutput->isChecked();
    bool isConnectable = ui->checkBoxConnectable->isChecked();
    bool isStorable = ui->checkBoxStorable->isChecked();

    if(!plugName.empty()){
        Sww::Core::PlugBase* p = Sww::Core::PlugRegister::create(plugTypeName,plugName);
        p->setOutput(isOutput);
        p->setConnectable(isConnectable);
        p->setStorable(isStorable);
        p->setUserDefined(true);
        _node->addDynamicPlug(p);
    }else{
        SWW_ERR("invalid node name.");
    }

    close();
}

void AddDynamicPlugWidget::on_pushButtonCencel_clicked()
{
    close();
}
