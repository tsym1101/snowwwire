#include "Geom/GeomEntry.h"
#include "Core/NodeRegister.h"
#include "Geom/Camera.h"
#include "Geom/OrthCamera.h"
#include "Geom/TileCamera.h"
#include "Geom/Transform.h"
#include "Geom/NdQuad.h"
#include "Geom/NdPointLight.h"
#include "Geom/NdDirectionalLight.h"
#include "Geom/NdSpotLight.h"
#include "Geom/NdCube.h"
#include "Geom/NdExtractVP.h"
#include "Geom/NdInstance.h"
#include "Geom/NdAxisAlignedPointsToBoundingMesh.h"
#include "Geom/NdTransformGeo.h"
#include "Geom/NdExtract2dImageFrom3dImage.h"

#include "Core/PlugRegister.h"

namespace SwwGeom {

bool entry(){
    SWW_CHECK_BOOL(Sww::Core::NodeRegister::registerNode("Camera",SwwGeom::Camera::creator,"Geometry"));
    SWW_CHECK_BOOL(Sww::Core::NodeRegister::registerNode("OrthCamera",SwwGeom::OrthCamera::creator,"Geometry"));
    SWW_CHECK_BOOL(Sww::Core::NodeRegister::registerNode("TileCamera",SwwGeom::TileCamera::creator,"Geometry"));
    SWW_CHECK_BOOL(Sww::Core::NodeRegister::registerNode("Transform",SwwGeom::Transform::creator,"Geometry"));
    SWW_CHECK_BOOL(Sww::Core::NodeRegister::registerNode("Quad",SwwGeom::NdQuad::creator,"Geometry"));
    SWW_CHECK_BOOL(Sww::Core::NodeRegister::registerNode("Cube",SwwGeom::NdCube::creator,"Geometry"));
    SWW_CHECK_BOOL(Sww::Core::NodeRegister::registerNode("PointLight",SwwGeom::NdPointLight::creator,"Geometry"));
    SWW_CHECK_BOOL(Sww::Core::NodeRegister::registerNode("DirectionalLight",SwwGeom::NdDirectionalLight::creator,"Geometry"));
    SWW_CHECK_BOOL(Sww::Core::NodeRegister::registerNode("SpotLight",SwwGeom::NdSpotLight::creator,"Geometry"));
    SWW_CHECK_BOOL(Sww::Core::NodeRegister::registerNode("ExtractVP",SwwGeom::NdExtractVP::creator,"Geometry"));
    SWW_CHECK_BOOL(Sww::Core::NodeRegister::registerNode("Instance",SwwGeom::NdInstance::creator,"Geometry"));
    SWW_CHECK_BOOL(Sww::Core::NodeRegister::registerNode("AxisAlignedPointsToBoundingMesh",SwwGeom::NdAxisAlignedPointsToBoundingMesh::creator,"Geometry"));
    SWW_CHECK_BOOL(Sww::Core::NodeRegister::registerNode("TransformGeo",SwwGeom::NdTransformGeo::creator,"Geometry"));
    SWW_CHECK_BOOL(Sww::Core::NodeRegister::registerNode("Extract2dImageFrom3dImage",SwwGeom::NdExtract2dImageFrom3dImage::creator,"Geometry"));

    Sww::Core::PlugRegister::registerPlug("shape",ShapePlug::creator);
    Sww::Core::PlugRegister::registerPlug("mesh",MeshPlug::creator);
    Sww::Core::PlugRegister::registerPlug("points",PointsPlug::creator);
    Sww::Core::PlugRegister::registerPlug("lines",LinesPlug::creator);
    Sww::Core::PlugRegister::registerPlug("camera",CameraPlug::creator);
    Sww::Core::PlugRegister::registerPlug("light",LightPlug::creator);
    Sww::Core::PlugRegister::registerPlug("instance",InstancePlug::creator);
    Sww::Core::PlugRegister::registerPlug("floatImage",FloatImagePlug::creator);
    Sww::Core::PlugRegister::registerPlug("byteImage",ByteImagePlug::creator);
    Sww::Core::PlugRegister::registerPlug("halfImage",HalfImagePlug::creator);
    Sww::Core::PlugRegister::registerPlug("transferfunction",TransferFunctionPlug::creator);


    return true;
}



}//ns
