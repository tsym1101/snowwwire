#include "ConnectDialog.h"
#include "ui_ConnectDialog.h"

#include "Core/PlugBase.h"
#include "Core/Node.h"
#include <QFileInfo>
#include <QRegExp>

ConnectDialog::ConnectDialog(Sww::Core::PlugBase *p, QWidget *parent) :
    _plug(p),QWidget(parent),
    ui(new Ui::ConnectDialog)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    setWindowTitle(QString("connect dialog : ") + _plug->getDebugName().c_str());
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::on_pushButtonConnect_clicked()
{
    bool forward = ui->checkBoxForward->isChecked();
    QString path = ui->lineEditPlugPath->text();

    QRegExp reg("^\\$parent");
    path.replace(reg,QString(_plug->getNode()->getName().c_str()) + "/../");

    SWW_DEBUG(path.toStdString());

    Sww::Core::NodeBase* nb = _plug->getNode();
    Sww::Core::Node* n = dynamic_cast<Sww::Core::Node*>(nb);
    Q_ASSERT(n);

    Sww::Core::Node* parentNode= n->parentNode();

    QFileInfo inf(path);
    QString connNodeName = inf.baseName();
    QString connPlugName = inf.suffix();

    if(inf.path() != "."){
        connNodeName = inf.path() + "/" + connNodeName;
    }


    if(_plug->isOutput()){
        if(!parentNode->connect(_plug->getNode()->getName(),_plug->getName(),
                                   connNodeName.toStdString(),connPlugName.toStdString())){
            SWW_ERR("connect failed.");
        }else{
            SWW_INF("connect success.");
        }
    }else{
        if(!parentNode->connect(connNodeName.toStdString(),connPlugName.toStdString(),
                                   _plug->getNode()->getName(),_plug->getName() )){
            SWW_ERR("connect failed.");
        }else{
            SWW_INF("connect success.");
        }
    }

    //close();

}
