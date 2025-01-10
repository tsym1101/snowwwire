#include "NdExtract2dImageFrom3dImage.h"
#include "Geom/PlugTypeGeom.h".h"

namespace  SwwGeom{

NdExtract2dImageFrom3dImage::NdExtract2dImageFrom3dImage()
{
}

bool NdExtract2dImageFrom3dImage::initialize()
{
    _i3dImage = new SwwGeom::FloatImagePlug("i3dImage");
    _i3dImage->setConnectable(true);

    _o2dImage = new SwwGeom::FloatImagePlug("o2dImage");
    _o2dImage->setConnectable(true);
    _o2dImage->setStorable(true);
    _o2dImage->setOutput(true);

    _z = new Sww::Core::IntPlug("z",0);
    _z->setStorable(true);
    _z->setConnectable(true);
    _z->setMin(0);

    addPlug(_i3dImage);
    addPlug(_z);
    addPlug(_o2dImage);

    _z->affects(_o2dImage);
    _i3dImage->affects(_o2dImage);

    return true;
}

QString NdExtract2dImageFrom3dImage::toolTip()const
{
    QString str = "3dImageから任意のz値の2dImageを抽出";
    return str;
}

Sww::Core::Node* NdExtract2dImageFrom3dImage::creator(){
    return new NdExtract2dImageFrom3dImage();
}

bool NdExtract2dImageFrom3dImage::execute(const Sww::Core::PlugBase *plug)
{
    SWW_VAL(_z,z);
    SWW_VAL(_i3dImage,i3dImage);

    if(!i3dImage){
        SWW_ERR("empty image. " << plug->getDebugName());
        return false;
    }

    if(z > i3dImage->depth() - 1){
        SWW_ERR("z is out of range. i3dImage depth = " << i3dImage->depth()
               << " " << plug->getDebugName());
        return false;
    }

    float* pixels = new float[i3dImage->width() * i3dImage->height() * i3dImage->channels()];

    int w = i3dImage->width();
    int h = i3dImage->height();
    int channels = i3dImage->channels();

    memcpy(pixels,
           &(i3dImage->pixels()[w * h * channels * z]),
            sizeof(float) * w * h * channels);

    SwwGeom::FloatImage::Ptr oImage(new SwwGeom::FloatImage(pixels,
                                                          w,
                                                          h,
                                                          channels,
                                                          1));

    _o2dImage->setOutputValue(oImage);

    return true;
}

}//ns SwwGeom
