#include "Geom/PlugTypeGeom.h"

namespace SwwGeom {

Sww::Core::PlugColor ShapePlug::getPlugColor()const{
    return Sww::Core::PlugColor(220,220,220);
}

bool ShapePlug::match(PlugBase *plug){

    bool valid =
            (plug->getSwwTypeDesc() == SwwGeom::SHAPE) ||
            (plug->getSwwTypeDesc() == SwwGeom::MESH) ||
            (plug->getSwwTypeDesc() == SwwGeom::POINTS) ||
            (plug->getSwwTypeDesc() == SwwGeom::LINES) ||
            (plug->getSwwTypeDesc() == SwwGeom::INSTANCE) ;

    if(!valid)
    {
        SWW_ERR("type desc mismatch." << getDebugName().c_str() << "&" << getDebugName().c_str() );
        return false;
    }
    return true;
}

Sww::Core::PlugColor MeshPlug::getPlugColor()const{
    return Sww::Core::PlugColor(0,10,80);
}

bool MeshPlug::match(PlugBase *plug){

    bool valid =
            (plug->getSwwTypeDesc() == SwwGeom::SHAPE) ||
            (plug->getSwwTypeDesc() == SwwGeom::MESH);

    if(!valid)
    {
        SWW_ERR("type desc mismatch." << getDebugName().c_str() << "&" << getDebugName().c_str() );
        return false;
    }
    return true;
}

Sww::Core::PlugColor PointsPlug::getPlugColor()const{
    return Sww::Core::PlugColor(210,250,250);
}

bool PointsPlug::match(PlugBase *plug){

    bool valid =
            (plug->getSwwTypeDesc() == SwwGeom::SHAPE) ||
            (plug->getSwwTypeDesc() == SwwGeom::POINTS);

    if(!valid)
    {
        SWW_ERR("type desc mismatch." << getDebugName().c_str() << "&" << getDebugName().c_str() );
        return false;
    }
    return true;
}


Sww::Core::PlugColor LinesPlug::getPlugColor()const{
    return Sww::Core::PlugColor(210,111,23);
}

bool LinesPlug::match(PlugBase *plug){

    bool valid =
            (plug->getSwwTypeDesc() == SwwGeom::SHAPE) ||
            (plug->getSwwTypeDesc() == SwwGeom::LINES);

    if(!valid)
    {
        SWW_ERR("type desc mismatch." << getDebugName().c_str() << "&" << getDebugName().c_str() );
        return false;
    }
    return true;
}

Sww::Core::PlugColor CameraPlug::getPlugColor()const{
    return Sww::Core::PlugColor(160,160,10);
}

Sww::Core::PlugColor LightPlug::getPlugColor()const{
    return Sww::Core::PlugColor(168,87,221);
}

Sww::Core::PlugColor InstancePlug::getPlugColor()const{
    return Sww::Core::PlugColor(62,186,26);
}

bool InstancePlug::match(PlugBase *plug){

    bool valid =
            (plug->getSwwTypeDesc() == SwwGeom::SHAPE) ||
            (plug->getSwwTypeDesc() == SwwGeom::INSTANCE);

    if(!valid)
    {
        SWW_ERR("type desc mismatch." << getDebugName().c_str() << "&" << getDebugName().c_str() );
        return false;
    }
    return true;
}

//******************************************************************************************************
TransferFunctionPlug::TransferFunctionPlug(const std::string &name, TransferFunctionPtr data)
    :Sww::Core::Plug<TransferFunctionPtr>(name,data)
{
    //setHidden(true);
}

SwwTypeDesc  TransferFunctionPlug::getSwwTypeDesc()const{return TRANSFERFUNCTION_DESC;}
std::string  TransferFunctionPlug::getSwwTypeDescString()const{return "transferfunction";}
Sww::Core::PlugColor  TransferFunctionPlug::getPlugColor()const{return Sww::Core::PlugColor(150,214,20);}
QString  TransferFunctionPlug::valueToString(){
    bool ok;
    TransferFunctionPtr ptr =  getConstInputValue(&ok);

    return ptr->valueToString();
}
bool TransferFunctionPlug::setValueString(const QString &valueStr){
    TransferFunctionPtr ptr(new TransFuncIntensity());
    ptr->setValueString(valueStr);
    bool result = setInputValue(ptr);
    return result;
}
//******************************************************************************************************


}//ns
