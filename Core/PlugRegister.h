#ifndef SWW_PLUGREGISTER_H
#define SWW_PLUGREGISTER_H
#include <string>
#include <QMap>
#include <vector>
#include "CoreAPI.h"
#include <QStringList>

SWW_CORE_NAMESPACE_BEGIN
class PlugBase;

typedef PlugBase* (*PlugCreatorFunction)(const std::string&);

class SWWCORESHARED_EXPORT PlugRegister
{
public:
    PlugRegister();

    static bool init();
    static PlugBase* create(const std::string &plugTypeName, const std::string &plugName);
    static void registerPlug(const std::string& plugTypeName,PlugCreatorFunction cFunc);

    static QStringList getAvailablePlugNames();
private:
    static QMap<std::string, PlugCreatorFunction> _creatorFunctionContainer;
};

SWW_CORE_NAMESPACE_END

#endif // SWW_PLUGREGISTER_H
