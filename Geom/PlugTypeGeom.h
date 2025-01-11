#ifndef PLUGTYPEIO_H
#define PLUGTYPEIO_H

#include "Geom/Mesh.h"
#include "Geom/Points.h"
#include "Geom/Lines.h"
#include "Geom/Camera.h"
#include "Geom/Light.h"
#include "Geom/Instance.h"

#include "Geom/Image.h"

#include "Core/PlugType.h"

#include "Geom/TransFuncIntensity.h"

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

class SWWGEOMSHARED_EXPORT  ShapePlug : public Sww::Core::Plug<Shape::ConstPtr>
{
public :
    typedef Shape::ConstPtr thisType;
    ShapePlug(const std::string& name,Shape::ConstPtr data = Shape::ConstPtr())
        :Sww::Core::Plug<Shape::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return SwwGeom::SHAPE;}
    virtual std::string getSwwTypeDescString()const{return "shape";}
    virtual Sww::Core::PlugColor getPlugColor()const;
    virtual bool match(PlugBase *plug);
    DECLARE_PLUG_CREATOR(ShapePlug)
};

class SWWGEOMSHARED_EXPORT  MeshPlug : public Sww::Core::Plug<Mesh::ConstPtr>
{
public :
    typedef Mesh::ConstPtr thisType;
    MeshPlug(const std::string& name,Mesh::ConstPtr data = Mesh::ConstPtr())
        :Sww::Core::Plug<Mesh::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return SwwGeom::MESH;}
    virtual std::string getSwwTypeDescString()const{return "mesh";}
    virtual Sww::Core::PlugColor getPlugColor()const;
    virtual bool match(PlugBase *plug);
    DECLARE_PLUG_CREATOR(MeshPlug)
};

class SWWGEOMSHARED_EXPORT  PointsPlug : public Sww::Core::Plug<Points::ConstPtr>
{
public :
    typedef Points::ConstPtr thisType;
    PointsPlug(const std::string& name,Points::ConstPtr data = Points::ConstPtr())
        :Sww::Core::Plug<Points::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return SwwGeom::POINTS;}
    virtual std::string getSwwTypeDescString()const{return "points";}
    virtual Sww::Core::PlugColor getPlugColor()const;
    virtual bool match(PlugBase *plug);
    DECLARE_PLUG_CREATOR(PointsPlug)
};

class SWWGEOMSHARED_EXPORT  LinesPlug : public Sww::Core::Plug<Lines::ConstPtr>
{
public :
    typedef Lines::ConstPtr thisType;
    LinesPlug(const std::string& name,Lines::ConstPtr data = Lines::ConstPtr())
        :Sww::Core::Plug<Lines::ConstPtr>(name,data)
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

class SWWGEOMSHARED_EXPORT LightPlug : public Sww::Core::Plug<Light::ConstPtr>
{
public :
    typedef Light::ConstPtr thisType;
    LightPlug(const std::string& name,Light::ConstPtr data = Light::ConstPtr())
        :Sww::Core::Plug<Light::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return SwwGeom::LIGHT;}
    virtual std::string getSwwTypeDescString()const{return "light";}
    virtual Sww::Core::PlugColor getPlugColor()const;
    DECLARE_PLUG_CREATOR(LightPlug)
};

class SWWGEOMSHARED_EXPORT InstancePlug : public Sww::Core::Plug<Instance::ConstPtr>
{
public :
    typedef Instance::ConstPtr thisType;
    InstancePlug(const std::string& name,Instance::ConstPtr data = Instance::ConstPtr())
        :Sww::Core::Plug<Instance::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return SwwGeom::INSTANCE;}
    virtual std::string getSwwTypeDescString()const{return "instance";}
    virtual Sww::Core::PlugColor getPlugColor()const;
    virtual bool match(PlugBase *plug);
    DECLARE_PLUG_CREATOR(InstancePlug)
};



class  FloatImagePlug : public Sww::Core::Plug<FloatImage::ConstPtr>
{
public :
    typedef FloatImage::ConstPtr thisType;
    FloatImagePlug(const std::string& name = "noName",FloatImage::ConstPtr data = FloatImage::ConstPtr())
        :Sww::Core::Plug<FloatImage::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return FLOATIMAGE_DESC;}
    virtual std::string getSwwTypeDescString()const{return "floatImage";}
    virtual Sww::Core::PlugColor getPlugColor()const{return Sww::Core::PlugColor(20,10,110);}

    DECLARE_PLUG_CREATOR(FloatImagePlug)
};

class  ByteImagePlug : public Sww::Core::Plug<ByteImage::ConstPtr>
{
public :
    typedef ByteImage::ConstPtr thisType;
    ByteImagePlug(const std::string& name = "noName",ByteImage::ConstPtr data = ByteImage::ConstPtr())
        :Sww::Core::Plug<ByteImage::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return BYTEIMAGE_DESC;}
    virtual std::string getSwwTypeDescString()const{return "byteImage";}
    virtual Sww::Core::PlugColor getPlugColor()const{return Sww::Core::PlugColor(20,200,110);}
    DECLARE_PLUG_CREATOR(ByteImagePlug)
};

class  HalfImagePlug : public Sww::Core::Plug<HalfImage::ConstPtr>
{
public :
    typedef HalfImage::ConstPtr thisType;
    HalfImagePlug(const std::string& name = "noName",HalfImage::ConstPtr data = HalfImage::ConstPtr())
        :Sww::Core::Plug<HalfImage::ConstPtr>(name,data)
    {setHidden(true);}
    virtual SwwTypeDesc getSwwTypeDesc()const{return HALFIMAGE_DESC;}
    virtual std::string getSwwTypeDescString()const{return "halfImage";}
    virtual Sww::Core::PlugColor getPlugColor()const{return Sww::Core::PlugColor(190,210,10);}
    DECLARE_PLUG_CREATOR(HalfImagePlug)
};


typedef std::shared_ptr<TransFuncIntensity> TransferFunctionPtr;
class  SWWGEOMSHARED_EXPORT TransferFunctionPlug : public Sww::Core::Plug<TransferFunctionPtr>
{
public :
    TransferFunctionPlug(const std::string& name,
                         TransferFunctionPtr data = TransferFunctionPtr(new TransFuncIntensity()));
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
