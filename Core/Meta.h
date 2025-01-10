#ifndef SWW_META_H
#define SWW_META_H

#include <string>
#include "Core/CoreAPI.h"

SWW_CORE_NAMESPACE_BEGIN

class SWWCORESHARED_EXPORT Meta
{
public:
    Meta();
    virtual ~Meta(){}
    const std::string& getName()const{return _name;}
    void setName(const std::string& name){_name = name;}
private:
    std::string _name;
};

SWW_CORE_NAMESPACE_END

#endif // SWW_META_H
