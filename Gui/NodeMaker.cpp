#include "NodeMaker.h"
#include "Gui/NodeItem.h"
#include "Gui/NodeGraphWidget.h"
#include "Core/NodeRegister.h"

using namespace SwwGui;

QPointF NodeMaker::_mousePos;

NodeMaker::NodeMaker(const QString &nodeName,NodeGraphWidget *parent) :
    _nodeName(nodeName),_ngw(parent),QObject(parent)
{

}

void NodeMaker::createNode()
{
    _ngw->createNode(_nodeName,_mousePos);
}


void NodeMaker::createMod()
{
    const std::string& jsonStr = Sww::Core::NodeRegister::getModByName(_nodeName.toStdString());
    _ngw->createMod(jsonStr);
}
