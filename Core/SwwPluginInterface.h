#ifndef SwwPluginInterface_H
#define SwwPluginInterface_H

#include "Core/CoreAPI.h"
#include <QString>
#include "SwwTypeDesc.h"

SWW_CORE_NAMESPACE_BEGIN

class PluginMetaInfo{
public:
    PluginMetaInfo(const std::string& name,const std::string& author,float version):
        _name(name),_author(author),_version(version)
    {}
    const std::string& name()const{return _name;}
    const std::string& author()const{return _author;}
    float version()const{return _version;}
private:
    std::string _name;
    std::string _author;
    float _version;
};

class  PluginInterface{
public:
    PluginInterface(){}
    virtual ~PluginInterface(){}
    virtual bool entry()const = 0;
    virtual bool cleanup()const{return true;}
    virtual PluginMetaInfo metaInfo()const = 0;
};

SWW_CORE_NAMESPACE_END

Q_DECLARE_INTERFACE(Sww::Core::PluginInterface,
                    "sww.PluginInterface/1.0")


#endif // SwwPluginInterface_H
