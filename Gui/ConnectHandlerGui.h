#ifndef CONNECTHANDLERGUI_H
#define CONNECTHANDLERGUI_H

#include "Gui/GuiAPI.h"
#include "Core/ConnectHandler.h"

namespace SwwGui {

class NodeGraphScene;

class SWWGUISHARED_EXPORT ConnectHandlerGui : public Sww::Core::ConnectHandler
{
public:
    ConnectHandlerGui(const std::string& outNode,const std::string& outPlug,
                   const std::string& inputNode,const std::string& inputPlug,
                   SwwGui::NodeGraphScene* scene);
    virtual bool exec();

private:
    SwwGui::NodeGraphScene* _nodeGraphScene;
};

}//ns

#endif // CONNECTHANDLERGUI_H
