#ifndef SWW_NODEREGISTER_H
#define SWW_NODEREGISTER_H

#include "CoreAPI.h"
#include <string>
#include <map>
#include <vector>
#include <QMap>

SWW_CORE_NAMESPACE_BEGIN

class Node;
typedef Node* (*CreatorFunction)();

class SWWCORESHARED_EXPORT NodeRegister
{
public:
    static bool create(const std::string &nodeName,const std::string &nodeType,Node*& node,Node* parentNode);     //ノードのインスタンスを生成
    static bool registerNode(const std::string& nodeName,
                             CreatorFunction cFunc,
                             const std::string& category = "none");
    static std::vector<std::string> getNodeNames();
    static std::vector<std::string> getModNames();
    static std::vector<std::string> getCategorys();
    static std::string& getCategory(const std::string& nodeName);

    static bool registerMod(const std::string& modName,
                            const std::string& jsonStr,
                            const std::string& category = "none");
    static std::string getModByName(const std::string &modName);

private:
    static std::map<std::string, CreatorFunction> _creatorFunctionContainer;
    static std::map<std::string,std::string> _categorys;
    static QMap<std::string,std::string> _modContainer;
};

SWW_CORE_NAMESPACE_END
#endif // SWW_NODEREGISTER_H
