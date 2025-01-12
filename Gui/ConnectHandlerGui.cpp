#include "ConnectHandlerGui.h"
#include "Gui/NodeGraphScene.h"
//#include <GL/glew.h>
#include "Core/SwwAssert.h"

namespace SwwGui {
ConnectHandlerGui::ConnectHandlerGui(const std::string &outNode, const std::string &outPlug,
                               const std::string &inputNode, const std::string &inputPlug,
                               SwwGui::NodeGraphScene *scene):
    ConnectHandler(outNode,outPlug,inputNode,inputPlug,scene->node()),_nodeGraphScene(scene)
{
}

bool ConnectHandlerGui::exec()
{
//    SWW_DEBUG("connect arg1:" <<  _outNodeName.c_str());
//    SWW_DEBUG("connect arg2:" <<  _outPlugName.c_str());
//    SWW_DEBUG("connect arg3:" <<  _inNodeName.c_str());
//    SWW_DEBUG("connect arg4:" <<  _inPlugName.c_str());
    return _nodeGraphScene->connect(_outNodeName.c_str(),
                                    _outPlugName.c_str(),
                                    _inNodeName.c_str(),
                                    _inPlugName.c_str());
}

}//ns
