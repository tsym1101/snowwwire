#include "ConnectHandler.h"

#include "NodeGraph.h"
#include "Core/Node.h"
#include "Core/PlugBase.h"

SWW_CORE_NAMESPACE_BEGIN

//ConnectHandler::ConnectHandler(Node *outNode, int outIndex, Node *inNode, int inIndex):
//    _outNode(outNode),_outputPlugIndex(outIndex),_inNode(inNode),_inputPlugIndex(inIndex),
//    _mode(ConnectHandler::INDEX),_parentNode(0)
//{

//}

ConnectHandler::ConnectHandler(const std::string &outNode, const std::string &outPlug,
                               const std::string &inputNode, const std::string &inputPlug,
                               Node* parentNode):
    _outNodeName(outNode),_outPlugName(outPlug),_inNodeName(inputNode),_inPlugName(inputPlug),
    _mode(ConnectHandler::STRING),_parentNode(parentNode)
{

}

ConnectHandler::ConnectHandler(PlugBase* outPlug,PlugBase* inPlug):
    _outPlug(outPlug),_inPlug(inPlug),
    _mode(ConnectHandler::PLUG),_parentNode(0)
{

}


bool ConnectHandler::exec()
{
    switch(_mode)
    {
//    case INDEX:
//        if(!Node::NodeGraph::connect(_outNode,_outputPlugIndex,_inNode,_inputPlugIndex)){
//            SWW_ERR("connect by handler failed. " << _outNode->getName() << " -> " << _inNode->getName());
//            return false;
//        }
//        break;
    case STRING:
        if(!_parentNode->connect(_outNodeName,_outPlugName,_inNodeName,_inPlugName)){
            SWW_ERR("connect by handler failed. " << _outNodeName << "." << _outPlugName << " -> " << _inNodeName << "." << _inPlugName);
            return false;
        }
        break;
    case PLUG:
        if(!Node::connect(_outPlug,_inPlug)){
            SWW_ERR("connect by handler failed. " << _outPlug->getDebugName() << " -> " << _inPlug->getDebugName());
            return false;
        }
        break;

    default:
        return false;
    }

    return true;

}

SWW_CORE_NAMESPACE_END
