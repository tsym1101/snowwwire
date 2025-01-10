#include "PlugRegister.h"
#include "Core/SwwAssert.h"
#include "Core/PlugBase.h"
#include "Core/PlugType.h"

SWW_CORE_NAMESPACE_BEGIN

QMap<std::string, PlugCreatorFunction> PlugRegister::_creatorFunctionContainer;

PlugRegister::PlugRegister()
{
}

PlugBase *PlugRegister::create(const std::string &plugTypeName, const std::string &plugName){
    PlugCreatorFunction cFunc = _creatorFunctionContainer.value(plugTypeName);

    if(!cFunc)
    {
        SWW_ERR(plugTypeName << " not found. ");
        return 0;
    }

    Sww::Core::PlugBase* p = (*cFunc)(plugName);
    return p;
}

void PlugRegister::registerPlug(const std::string& plugTypeName, PlugCreatorFunction cFunc){
    Q_ASSERT(cFunc);
    Q_ASSERT_X(!_creatorFunctionContainer.contains(plugTypeName),
               plugTypeName.c_str(),
               "already exist.");

    _creatorFunctionContainer[plugTypeName] = cFunc;
}


bool PlugRegister::init(){
    registerPlug("bool",BoolPlug::creator);
    registerPlug("int",IntPlug::creator);
    registerPlug("v2i",V2iPlug::creator);
    registerPlug("v3i",V3iPlug::creator);
    registerPlug("float",FloatPlug::creator);
    registerPlug("v2f",V2fPlug::creator);
    registerPlug("v3f",V3fPlug::creator);
    registerPlug("v4f",V4fPlug::creator);
    registerPlug("v3d",V3dPlug::creator);
    registerPlug("m33f",M33fPlug::creator);
    registerPlug("m44f",M44fPlug::creator);
    registerPlug("box3f",Box3fPlug::creator);
    registerPlug("null",NullPlug::creator);
    registerPlug("void",VoidPlug::creator);
    registerPlug("string",StringPlug::creator);
    registerPlug("stringArray",StringArrayPlug::creator);
    registerPlug("file",FilePlug::creator);
    registerPlug("multiFile",MultiFilePlug::creator);
    registerPlug("enum",EnumPlug::creator);
    registerPlug("enumString",EnumStringPlug::creator);
    registerPlug("floatArray",FloatArrayPlug::creator);
    registerPlug("v2fArray",V2fArrayPlug::creator);
    registerPlug("v3fArray",V3fArrayPlug::creator);
    registerPlug("v4fArray",V4fArrayPlug::creator);
    registerPlug("intArray",IntArrayPlug::creator);
    registerPlug("uint32Array",Uint32ArrayPlug::creator);
    registerPlug("uint8Array",Uint8ArrayPlug::creator);

    return true;
}

QStringList PlugRegister::getAvailablePlugNames(){
    QStringList result;
    QMap<std::string, PlugCreatorFunction>::iterator i = _creatorFunctionContainer.begin();
    while (i != _creatorFunctionContainer.end()) {
       result.push_back(i.key().c_str());
        ++i;
    }
    return result;
}

SWW_CORE_NAMESPACE_END
