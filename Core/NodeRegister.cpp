#include "NodeRegister.h"
#include "Node.h"
#include "Core/VectorHelper.h"

SWW_CORE_NAMESPACE_BEGIN

std::map<std::string, CreatorFunction>  NodeRegister::_creatorFunctionContainer;
std::map<std::string, std::string>  NodeRegister::_categorys;
QMap<std::string, std::string>  NodeRegister::_modContainer;

bool NodeRegister::registerNode(const std::string& nodeName,
                                CreatorFunction cFunc,
                                const std::string& category)
{
    Q_ASSERT(cFunc);
    Q_ASSERT_X(_creatorFunctionContainer.end() == _creatorFunctionContainer.find(nodeName),
               nodeName.c_str(),
               "already exist.");
    Q_ASSERT_X(!_modContainer.contains(nodeName),
               nodeName.c_str(),
               "already exist.");

    _creatorFunctionContainer[nodeName] = cFunc;
    _categorys[nodeName] = category;

    return true;
}

bool NodeRegister::registerMod(const std::string &modName, const std::string &jsonStr, const std::string &category){
    Q_ASSERT_X(_creatorFunctionContainer.end() == _creatorFunctionContainer.find(modName),
               modName.c_str(),
               "already exist.");
    Q_ASSERT_X(!_modContainer.contains(modName),
               modName.c_str(),
               "already exist.");

    _modContainer[modName] = jsonStr;
    _categorys[modName] = category;

    return true;
}

std::string NodeRegister::getModByName(const std::string& modName){
    return _modContainer.value(modName);
}

bool NodeRegister::create(const std::string &nodeName, const std::string &nodeType, Node *&node, Node *parentNode)
{
    CreatorFunction cFunc = _creatorFunctionContainer[nodeType];

    if(!cFunc) //名前がなかったら0を返す
    {
        SWW_ERR(nodeType << " not found. ");
        return false;
    }

    node = (*cFunc)();
    //node->setParentGraph(graph);//親のノードグラフをセット

    Q_ASSERT( parentNode->_rootNode);
    node->_rootNode = parentNode->_rootNode;
    node->_parentNode = parentNode;
    node->setName(nodeName);
    node->nodeTypeName(nodeType);

    SWW_CHECK_BOOL(node->initialize());
    node->setInitialized();

    return true;
}

std::vector<std::string> NodeRegister::getNodeNames()
{
    std::vector<std::string> result;
    for(std::map<std::string, CreatorFunction>::const_iterator i = _creatorFunctionContainer.begin();
        i != _creatorFunctionContainer.end();
        ++i)
    {
        std::string name = i->first;
        result.push_back(name);
    }

    std::sort(result.begin(),result.end());
    return result;
}

std::vector<std::string> NodeRegister::getModNames()
{
    std::vector<std::string> result;
    for(QMap<std::string,std::string>::const_iterator i = _modContainer.begin();
        i != _modContainer.end();
        ++i)
    {
        std::string name = i.key();
        result.push_back(name);
    }

    std::sort(result.begin(),result.end());
    return result;
}

std::vector<std::string> NodeRegister::getCategorys()
{
    std::vector<std::string> result;
    for(std::map<std::string, std::string>::const_iterator i = _categorys.begin();
        i != _categorys.end();
        ++i)
    {
        std::string name = i->second;
        if(name == "global") continue;
        if(!vectorContains(result,name))result.push_back(name);
    }

    std::sort(result.begin(),result.end());
    return result;
}

std::string& NodeRegister::getCategory(const std::string& nodeName)
{
    return _categorys[nodeName];
}

SWW_CORE_NAMESPACE_END
