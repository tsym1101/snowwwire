#include "IOUtility.h"

#include <QRegExp>
#include <QRegularExpression>
#include <QDir>
#include <QFileInfo>
#include "SwwNsUtil.h"
#include "Core/SwwAssert.h" //only header

#ifdef USE_OIIO
#include <OpenImageIO/imageio.h>
#include <Imath/half.h>
#endif

#include <typeinfo>

#ifdef _WIN32
#include <windows.h> //from Qt6 with vc2022
#include <VersionHelpers.h>
#endif


#ifdef USE_OIIO
namespace IOUtilLocal {

//local function
template <class T>
bool loadImage( const std::string &fileName,T*& pixels,int& width, int& height, int &channels, bool flip = false){
    OIIO_NAMESPACE_USING;

    OIIO_NAMESPACE::TypeDesc desc = Sww::Util::IOUtility::getOiioTypeDesc<T>();
    Q_ASSERT_X(desc != TypeDesc::UNKNOWN,fileName.c_str(),"load image failed.");

//    auto in = ImageInput::create(fileName);
//    if (! in){
//        SWW_ERR("image readeng failed. : " << fileName);
//        return false;
//    }

//    ImageSpec spec;
//    if(! in->open (fileName, &spec)){
//        SWW_ERR("image readeng failed. : " << fileName);
//        return false;
//    }

    auto in = ImageInput::open(fileName);
    if (! in){
          SWW_ERR("image readeng failed. : " << fileName);
          return false;
    }

    const ImageSpec &spec = in->spec();

    width = spec.full_width;//本来の解像度幅
    height = spec.full_height;//本来の解像度高さ
    int startX = spec.x;//ROI左端
    int startY = spec.y;//ROI上端
    int xres = spec.width; //実データが格納されているROIの幅
    int yres = spec.height; //実データが格納されているROIの高さ

    if(spec.full_width != spec.width || spec.full_height != spec.height){
        SWW_WARN("image has ROI. [full " << spec.full_width << "," << spec.full_height <<
                " : ROI " << spec.width << "," << spec.height <<  "] " << fileName);
    }

    channels = spec.nchannels;
    pixels = new T [xres*yres*channels];

    if(!flip){
        if(! in->read_image(desc, pixels))
        {
            SWW_ERR("error... image reading failed.check image file format. : " << fileName);
            delete[] pixels;
            pixels = 0;
            in->close();
//            ImageInput::destroy(in);
            return false;
        }
    }else{
        int scanlineSize = width * channels;
        if(! in->read_image(desc,
                            pixels + ((height-1) * scanlineSize),//先頭ピクセルのポインタ
                            sizeof(T) * channels, //X stride.
                            -scanlineSize * sizeof(T)))//Y stride.
        {
            SWW_ERR("error... image reading failed.check image file format. : " << fileName);
            delete[] pixels;
            pixels = 0;
            in->close();
//            ImageInput::destroy(in);
            return false;
        }
    }

    if(!in->close()){
        SWW_ERR("image close failed. : " << fileName);
//        ImageInput::destroy(in);
        return false;
    }

    return true;
}

//local function
template <class T>
bool saveImage(const std::string &fileName,T* pixels,int width,int height,int channels,bool flip,
               OIIO_NAMESPACE::TypeDesc oTypeDesc)
{
    OIIO_NAMESPACE_USING;

    OIIO_NAMESPACE::TypeDesc desc =  Sww::Util::IOUtility::getOiioTypeDesc<T>();

    if(!Sww::Util::IOUtility::makeDir(fileName))return false;

    Q_ASSERT_X(desc != TypeDesc::UNKNOWN,fileName.c_str(),"save image failed.");
    Q_ASSERT_X(oTypeDesc == TypeDesc::FLOAT || oTypeDesc == TypeDesc::HALF || oTypeDesc == TypeDesc::UCHAR
               ,fileName.c_str(),"unsupported bit depth.");


    auto out = ImageOutput::create(fileName);
    if (! out)return false;


    ImageSpec spec (width,height,channels,oTypeDesc);

    //拡張子ごとの処理---------------------------------------------------------
    QFileInfo info(fileName.c_str());
    QString suffix = info.completeSuffix();

    if(suffix == "tif" ||suffix == "tiff")
    {
        spec.attribute("compression","lzw");
        spec.attribute ("oiio:ColorSpace","sRGB");
    }else if(suffix == "exr")
    {
        spec.attribute("compression","zip");
    }else
    {
        //do nothing
    }
    //---------------------------------------------------------------------

    if(!out->open(fileName, spec))return false;

    bool result;
    if(flip)
    {
        //書き出し 上下フリップして書き出す
        int scanlineSize = width * channels;
        result =  out->write_image(desc,
                                   pixels + ((height-1) * scanlineSize),//先頭ピクセルのポインタ
                                   sizeof(T) * channels, //X stride.
                                   -scanlineSize * sizeof(T));//Y stride.
    }else{
        //書き出し
        result =  out->write_image (desc, pixels);
    }

    out->close ();
//    ImageOutput::destroy(out);



    return result;
}

}//ns
#endif

SWW_UTIL_NAMESPACE_BEGIN

std::string IOUtility::replaceFileNumber(int frame,const std::string &fileName)
{
    QString str = fileName.c_str();

    QRegularExpression rStd("%0\dd");
    if(str.contains(rStd))return replaceFileNumberStd(frame,fileName);

    QRegExp r("#+");    //正規表現 #+ -> #の１回以上の繰り返し
    r.indexIn(str);   //#を検索

    QString numStr = QString::number(frame);
    numStr = numStr.rightJustified(r.matchedLength(),'0');   //#の桁数に合うように0で埋める

//    return str.replace(r,numStr).toStdString();
    return str.replace(QRegularExpression("#+"),numStr).toStdString();
}
std::string IOUtility::replaceFileNumberStd(int frame, const std::string &fileName)
{
    char outputChar[1024];
    sprintf(outputChar,fileName.c_str(),frame);
    return  std::string(outputChar);
}

std::string IOUtility::makeSeaqenceName(const std::string &fileName)
{
    QFileInfo inf(fileName.c_str());
    QString baseName = inf.completeBaseName();

    QString pattern = "\\d+$";      //末尾の数字部分を指定する正規表現　\\d→数字　+→１回以上の繰り返し　$→末尾

    QRegularExpression r(pattern);
    QString baseNameWithoutNumber(baseName);
    baseNameWithoutNumber.remove(r);    //数字部分削除
    QString number(baseName);
    number.remove(baseNameWithoutNumber);   //数字部分抽出

    QString sharp = "";
    sharp = sharp.rightJustified(number.length(),'#');

    QString result = baseNameWithoutNumber + sharp + "." + inf.suffix();
    result = inf.absolutePath() + "/" + result;

    return result.toStdString();
}

std::string IOUtility::replaceCameraName(int num,const std::string &fileName)
{
    QString numStr = QString::number(num);
    numStr = numStr.rightJustified(3,'0');
    numStr.insert(0,"cam");
    QString fileNameQt = fileName.c_str();
    fileNameQt.replace("%cam",numStr);
    return fileNameQt.toStdString();
}

bool IOUtility::makeDir(const std::string &fileName)
{
    //ディレクトリなかったら生成
    QDir dir =  QFileInfo(fileName.c_str()).dir();
    if(!dir.exists())
    {
        if(! dir.mkpath(dir.path())){
            SWW_ERR("error... mkdir failed. " << fileName);
            return false;
        }
    }
    return true;
}

#ifdef USE_OIIO
bool IOUtility::saveImage(const std::string &fileName,float* pixels, int width,int height,int channels,bool flip,
                          OIIO_NAMESPACE::TypeDesc desc)
{
    return   IOUtilLocal::saveImage(fileName,pixels,width,height,channels,flip,desc);
}

bool IOUtility::saveImage(const std::string &fileName,unsigned char* pixels, int width,int height,int channels,bool flip,
                          OIIO_NAMESPACE::TypeDesc desc)
{
    return   IOUtilLocal::saveImage(fileName,pixels,width,height,channels,flip,desc);
}

bool IOUtility::saveImage(const std::string &fileName,half* pixels, int width,int height,int channels,bool flip,
                          OIIO_NAMESPACE::TypeDesc desc)
{
    return   IOUtilLocal::saveImage(fileName,pixels,width,height,channels,flip,desc);
}

bool IOUtility::loadImage(const std::string& fileName, float*& pixels,int& width,int& height,int &channels ,bool flip){
    return IOUtilLocal::loadImage(fileName,pixels,width,height,channels,flip);
}

bool IOUtility::loadImage(const std::string& fileName, unsigned char*& pixels,int& width,int& height,int &channels ,bool flip){
    return IOUtilLocal::loadImage(fileName,pixels,width,height,channels,flip);
}


bool IOUtility::loadImage(const std::string& fileName, half *&pixels, int& width, int& height, int &channels , bool flip){
    return IOUtilLocal::loadImage(fileName,pixels,width,height,channels,flip);
}

bool IOUtility::getImageSpec(const std::string &fileName, OIIO_NAMESPACE::ImageSpec *spec)
{
    OIIO_NAMESPACE_USING;
    auto in = ImageInput::create (fileName);
    if (! in){
        SWW_ERR("image readeng failed. : " << fileName);
        return false;
    }

    if(! in->open (fileName, *spec)){
        SWW_ERR("image readeng failed. : " << fileName);
        return false;
    }

    in->close();
//    ImageInput::destroy(in);

    return true;
}

bool IOUtility::initOiio(){

#ifdef _WIN32

    if(!IsWindowsVersionOrGreater(10,0,0)){
        if(!OIIO::attribute("threads",1)){
            SWW_ERR("set  oiio threads failed.");
            return false;
        }else{
            SWW_INF("set oiio threads 1 on win7 or older.");
        }
    }

#endif

    return true;
}
#endif

SWW_UTIL_NAMESPACE_END
