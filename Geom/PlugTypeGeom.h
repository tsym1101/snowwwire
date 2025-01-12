#ifndef PLUGTYPEIO_H
#define PLUGTYPEIO_H

#include <memory>

#include "Geom/Mesh.h"
#include "Geom/Points.h"
#include "Geom/Lines.h"
#include "Geom/Light.h"
#include "Geom/Instance.h"
#include "Geom/Image.h"
#include "Core/PlugType.h"
#include "Geom/TransFuncIntensity.h"

#include "Geom/Camera.h"
#include "Geom/GeomAPI.h"

namespace SwwGeom {

const SwwTypeDesc GEOM_DESC = 0x00FF0 + 100;
const SwwTypeDesc SHAPE    =  GEOM_DESC;
const SwwTypeDesc MESH     =  GEOM_DESC + 1;
const SwwTypeDesc POINTS   =  GEOM_DESC + 2;
const SwwTypeDesc LINES    =  GEOM_DESC + 3;
const SwwTypeDesc CAMERA   =  GEOM_DESC + 4;
const SwwTypeDesc LIGHT    =  GEOM_DESC + 5;
const SwwTypeDesc INSTANCE =  GEOM_DESC + 6;

const SwwTypeDesc FLOATIMAGE_DESC = 0x00FF0 + 3;
const SwwTypeDesc BYTEIMAGE_DESC  = 0x00FF0 + 4;
const SwwTypeDesc HALFIMAGE_DESC  = 0x00FF0 + 5;

const SwwTypeDesc TRANSFERFUNCTION_DESC  = 0x00FF0 + 6;

class SWWGEOMSHARED_EXPORT  ShapePlug : public Sww::Core::Plug<SwwGeom::Shape::ConstPtr>
{
public :
    typedef SwwGeom::Shape::ConstPtr thisType;
    ShapePlug(const std::string& name,SwwGeom::Shape::ConstPtr data = SwwGeom::Shape::ConstPtr())
        :Sww::Core::Plug<SwwGeom::Shape::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return SwwGeom::SHAPE;}
    virtual std::string getSwwTypeDescString()const{return "shape";}
    virtual Sww::Core::PlugColor getPlugColor()const;
    virtual bool match(PlugBase *plug);
    DECLARE_PLUG_CREATOR(ShapePlug)
};

class SWWGEOMSHARED_EXPORT  MeshPlug : public Sww::Core::Plug<SwwGeom::Mesh::ConstPtr>
{
public :
    typedef SwwGeom::Mesh::ConstPtr thisType;
    MeshPlug(const std::string& name,SwwGeom::Mesh::ConstPtr data = SwwGeom::Mesh::ConstPtr())
        :Sww::Core::Plug<SwwGeom::Mesh::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return SwwGeom::MESH;}
    virtual std::string getSwwTypeDescString()const{return "mesh";}
    virtual Sww::Core::PlugColor getPlugColor()const;
    virtual bool match(PlugBase *plug);
    DECLARE_PLUG_CREATOR(MeshPlug)
};

class SWWGEOMSHARED_EXPORT  PointsPlug : public Sww::Core::Plug<SwwGeom::Points::ConstPtr>
{
public :
    typedef SwwGeom::Points::ConstPtr thisType;
    PointsPlug(const std::string& name,SwwGeom::Points::ConstPtr data = SwwGeom::Points::ConstPtr())
        :Sww::Core::Plug<SwwGeom::Points::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return SwwGeom::POINTS;}
    virtual std::string getSwwTypeDescString()const{return "points";}
    virtual Sww::Core::PlugColor getPlugColor()const;
    virtual bool match(PlugBase *plug);
    DECLARE_PLUG_CREATOR(PointsPlug)
};

class SWWGEOMSHARED_EXPORT  LinesPlug : public Sww::Core::Plug<SwwGeom::Lines::ConstPtr>
{
public :
    typedef SwwGeom::Lines::ConstPtr thisType;
    LinesPlug(const std::string& name,SwwGeom::Lines::ConstPtr data = SwwGeom::Lines::ConstPtr())
        :Sww::Core::Plug<SwwGeom::Lines::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return SwwGeom::LINES;}
    virtual std::string getSwwTypeDescString()const{return "lines";}
    virtual Sww::Core::PlugColor getPlugColor()const;
    virtual bool match(PlugBase *plug);
    DECLARE_PLUG_CREATOR(LinesPlug)
};

class SWWGEOMSHARED_EXPORT CameraPlug : public Sww::Core::Plug<Camera*>
{
public :
    typedef Camera* thisType;
    CameraPlug(const std::string& name,Camera* data = 0)
        :Sww::Core::Plug<Camera*>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return SwwGeom::CAMERA;}
    virtual std::string getSwwTypeDescString()const{return "camera";}
    virtual Sww::Core::PlugColor getPlugColor()const;
    DECLARE_PLUG_CREATOR(CameraPlug)
};

class SWWGEOMSHARED_EXPORT LightPlug : public Sww::Core::Plug<SwwGeom::Light::ConstPtr>
{
public :
    typedef SwwGeom::Light::ConstPtr thisType;
    LightPlug(const std::string& name,SwwGeom::Light::ConstPtr data = SwwGeom::Light::ConstPtr())
        :Sww::Core::Plug<SwwGeom::Light::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return SwwGeom::LIGHT;}
    virtual std::string getSwwTypeDescString()const{return "light";}
    virtual Sww::Core::PlugColor getPlugColor()const;
    DECLARE_PLUG_CREATOR(LightPlug)
};

class SWWGEOMSHARED_EXPORT InstancePlug : public Sww::Core::Plug<SwwGeom::Instance::ConstPtr>
{
public :
    typedef SwwGeom::Instance::ConstPtr thisType;
    InstancePlug(const std::string& name,SwwGeom::Instance::ConstPtr data = SwwGeom::Instance::ConstPtr())
        :Sww::Core::Plug<SwwGeom::Instance::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return SwwGeom::INSTANCE;}
    virtual std::string getSwwTypeDescString()const{return "instance";}
    virtual Sww::Core::PlugColor getPlugColor()const;
    virtual bool match(PlugBase *plug);
    DECLARE_PLUG_CREATOR(InstancePlug)
};

class  FloatImagePlug : public Sww::Core::Plug<SwwGeom::FloatImage::ConstPtr>
{
public :
    typedef SwwGeom::FloatImage::ConstPtr thisType;
    FloatImagePlug(const std::string& name = "noName",SwwGeom::FloatImage::ConstPtr data = SwwGeom::FloatImage::ConstPtr())
        :Sww::Core::Plug<SwwGeom::FloatImage::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return FLOATIMAGE_DESC;}
    virtual std::string getSwwTypeDescString()const{return "floatImage";}
    virtual Sww::Core::PlugColor getPlugColor()const{return Sww::Core::PlugColor(20,10,110);}

    DECLARE_PLUG_CREATOR(FloatImagePlug)
};

class  ByteImagePlug : public Sww::Core::Plug<SwwGeom::ByteImage::ConstPtr>
{
public :
    typedef SwwGeom::ByteImage::ConstPtr thisType;
    ByteImagePlug(const std::string& name = "noName",SwwGeom::ByteImage::ConstPtr data = SwwGeom::ByteImage::ConstPtr())
        :Sww::Core::Plug<SwwGeom::ByteImage::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return BYTEIMAGE_DESC;}
    virtual std::string getSwwTypeDescString()const{return "byteImage";}
    virtual Sww::Core::PlugColor getPlugColor()const{return Sww::Core::PlugColor(20,200,110);}
    DECLARE_PLUG_CREATOR(ByteImagePlug)
};

class  HalfImagePlug : public Sww::Core::Plug<SwwGeom::HalfImage::ConstPtr>
{
public :
    typedef SwwGeom::HalfImage::ConstPtr thisType;
    HalfImagePlug(const std::string& name = "noName",SwwGeom::HalfImage::ConstPtr data = SwwGeom::HalfImage::ConstPtr())
        :Sww::Core::Plug<SwwGeom::HalfImage::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return HALFIMAGE_DESC;}
    virtual std::string getSwwTypeDescString()const{return "halfImage";}
    virtual Sww::Core::PlugColor getPlugColor()const{return Sww::Core::PlugColor(190,210,10);}
    DECLARE_PLUG_CREATOR(HalfImagePlug)
};


typedef std::shared_ptr<SwwGeom::TransFuncIntensity> TransferFunctionPtr;
class  SWWGEOMSHARED_EXPORT TransferFunctionPlug : public Sww::Core::Plug<TransferFunctionPtr>
{
public :
    TransferFunctionPlug(const std::string& name,
                         TransferFunctionPtr data = TransferFunctionPtr(new SwwGeom::TransFuncIntensity()));
    virtual SwwTypeDesc getSwwTypeDesc()const;
    virtual std::string getSwwTypeDescString()const;
    virtual Sww::Core::PlugColor getPlugColor()const;
    virtual QString valueToString();
    virtual bool setValueString(const QString &valueStr);
private:

public:
    DECLARE_PLUG_CREATOR(TransferFunctionPlug)
};


}//ns


#endif // PLUGTYPEIO_H
