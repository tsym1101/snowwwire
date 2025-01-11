#ifndef CONNECTHANDLER_H
#define CONNECTHANDLER_H

#include "CoreAPI.h"
#include <string>
#include "SwwTypeDef.h"

SWW_CORE_NAMESPACE_BEGIN

class Node;
class PlugBase;

class SWWCORESHARED_EXPORT ConnectHandler
{
    enum MODE{
//        INDEX,
        STRING,
        PLUG
    };

public:
    //ConnectHandler(Node* outNode,int outIndex,Node* inNode,int inIndex);
    ConnectHandler(PlugBase* outPlug,PlugBase* inPlug);

    ConnectHandler(const std::string& outNode,const std::string& outPlug,
                   const std::string& inputNode,const std::string& inputPlug ,
                   Node* parentNode);


    virtual bool exec();

protected:
    Node* _outNode;
    Node* _inNode;
    PlugBase* _inPlug;
    PlugBase* _outPlug;
    int _inputPlugIndex,_outputPlugIndex;
    std::string _inNodeName,_outNodeName,_inPlugName,_outPlugName;
    Node* _parentNode;
    MODE _mode;
};
SWW_CORE_NAMESPACE_END
#endif // CONNECTHANDLER_H
