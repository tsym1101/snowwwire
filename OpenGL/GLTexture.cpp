#include "GLTexture.h"

#include <QFileInfo>
#include <QDir>
#include <string>

#include "Core/SwwAssert.h"

#ifdef USE_OIIO
#include <OpenImageIO/imageio.h>
#include <opencv2/imgproc/imgproc.hpp>
#endif

//#include "Util/IOUtility.h"
#include "OpenGL/SwwGLMacro.h"
#include "Geom/Image.h"

//#include <opencv2/highgui/highgui.hpp>

namespace SwwOpenGL{

//std::mutex _mutex;

#ifdef USE_OIIO
OIIO_NAMESPACE::TypeDesc
glInternalFormatToOiioTypeDesc(GLint internalFormat )
{
    OIIO_NAMESPACE_USING;
    TypeDesc typeDesc;

    switch(internalFormat)
    {
    case GL_RGBA32F:
        typeDesc = TypeDesc::FLOAT;
        break;
    case GL_RGBA8:
        typeDesc = TypeDesc::UCHAR;
        break;
    case GL_DEPTH_COMPONENT32F:
        typeDesc = TypeDesc::FLOAT;
        break;
    default:
        SWW_ERR("internal format case statemaent not defined.");
        typeDesc = TypeDesc::UNKNOWN;
        break;
    }

    return typeDesc;
}
#endif

GLTexture::GLTexture(int width, int height, int depth,GLenum target,
                     GLint internalFormat,GLenum format,GLenum type
                     ,GLint filter ,GLint wrap)
    :_width(width),_height(height),_depth(depth),_target(target),
      _internalFormat(internalFormat),_format(format),_type(type),
      _filter(filter),_wrap(wrap),_data(0)
{
    //SWW_CHECK_GL_CONTEXT;
    init();
}

GLTexture::~GLTexture()
{
    glDeleteTextures(1,&_texId);

    if(_data){
        switch(_type){
        case GL_FLOAT:{
            delete [] (float*)_data;
            break;
        }
        case GL_UNSIGNED_BYTE:{
            delete [] (unsigned char*)_data;
            break;
        }
        }
    }
}

void GLTexture::init()
{
    glGenTextures( 1, &_texId );
    glBindTexture( _target, _texId );

    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, _filter);
    glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, _filter);
    glTexParameteri(_target, GL_TEXTURE_WRAP_S, _wrap);
    glTexParameteri(_target, GL_TEXTURE_WRAP_T, _wrap);

//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    uploadData(0);

    glBindTexture( _target, 0 );

}

void GLTexture::resize(int width , int height ,bool reset)
{
    _width = width;
    _height = height;

    if(reset)uploadData(0);
}

void GLTexture::uploadData(const void *data) const
{
    glBindTexture( _target, _texId );
    switch(_target) {
    case GL_TEXTURE_1D:
    {
        glTexImage1D( _target, 0, _internalFormat, _width,0,_format, _type, data );
        break;
    }
    case GL_TEXTURE_2D:{
        glTexImage2D( _target, 0, _internalFormat, _width, _height,0,_format, _type, data );
        break;
    }
    case GL_TEXTURE_2D_ARRAY:{
        glTexImage3D( _target, 0, _internalFormat, _width, _height,_depth,0,_format, _type, data );
        break;
    }
    case GL_TEXTURE_3D:{
        glTexImage3D( _target, 0, _internalFormat, _width, _height,_depth,0,_format, _type, data );
        break;
    }
    case GL_TEXTURE_RECTANGLE_ARB:{
        glTexImage2D( _target, 0, _internalFormat, _width, _height,0,_format, _type, data );
        break;
    }
    case GL_TEXTURE_CUBE_MAP:{
        for(int i=0; i<6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                         _internalFormat, _width,_height, 0,
                         _format, _type, data);
        }
        break;
    }
    }
    glBindTexture( _target, 0 );
}

void GLTexture::populateData(void *data)
{
    size_t size = _width * _height * _depth * numChannels();

    switch(_type){
    case GL_FLOAT:{
        float* dataTmp = new float[size];
        for(int i = 0;i < size;i++){
            dataTmp[i] = ((float*)data)[i];
        }
        _data = dataTmp;
        break;
    }
    case GL_UNSIGNED_BYTE:{
        unsigned char* dataTmp = new unsigned char[size];
        for(int i = 0;i < size;i++){
            dataTmp[i] = ((unsigned char*)data)[i];
        }
        _data = dataTmp;
        break;
    }
    default:{
        SWW_ERR("unsupported data type.");
        return;
        break;
    }
    }

    uploadData(_data);
}

void GLTexture::allocateData()
{
    size_t size = _width * _height * _depth * numChannels();

    switch(_type){
    case GL_FLOAT:{
        if(_data)delete [] (float*)_data;
        float* dataTmp = new float[size];
        _data = dataTmp;
        break;
    }
    case GL_UNSIGNED_BYTE:{
        if(_data)delete [] (unsigned char*)_data;
        unsigned char* dataTmp = new unsigned char[size];
        _data = dataTmp;
        break;
    }
    default:{
        SWW_ERR("unsupported data type.");
        return;
        break;
    }
    }

    uploadData(_data);
}

void GLTexture::uploadInternalData()const
{
    if(_data)uploadData(_data);
}
#ifdef USE_OIIO
bool GLTexture::saveAsFloat(const char *fileName) const
{
    OIIO_NAMESPACE::TypeDesc typeDesc = OIIO_NAMESPACE::TypeDesc::FLOAT;
    float* pixels = new float[_width * _height * numChannels()];
    glBindTexture( _target, _texId );
    glGetTexImage(_target,0,_format,_type,pixels);
    glBindTexture(  _target, 0);
    SwwGeom::FloatImage::Ptr iPtr(new SwwGeom::FloatImage(pixels,_width,_height,numChannels(),1));
    if(!iPtr->saveImage(fileName,true,typeDesc))return false;
    return true;
}

bool GLTexture::saveAsUint8(const char *fileName) const
{
    OIIO_NAMESPACE::TypeDesc typeDesc = OIIO_NAMESPACE::TypeDesc::UCHAR;
    uint8_t* pixels = new uint8_t[_width * _height * numChannels()];
    glBindTexture( _target, _texId );
    glGetTexImage(_target,0,_format,_type,pixels);
    glBindTexture(  _target, 0);
    SwwGeom::ByteImage::Ptr iPtr(new SwwGeom::ByteImage(pixels,_width,_height,numChannels(),1));
    if(!iPtr->saveImage(fileName,true,typeDesc))return false;
    return true;
}

bool GLTexture::save(const char *fileName, SAVE_TYPE type) const
{
    switch(_internalFormat){
    case(GL_RGBA32F):
    case(GL_RGB32F):
    case(GL_DEPTH_COMPONENT32F):
    {
        OIIO_NAMESPACE::TypeDesc typeDesc = OIIO_NAMESPACE::TypeDesc::FLOAT;

        //SWW_DEBUG_VALUE(numChannels());

        float* pixels = new float[_width * _height * numChannels()];
        glBindTexture( _target, _texId );
        glGetTexImage(_target,0,_format,_type,pixels);
        glBindTexture(  _target, 0);
        SwwGeom::FloatImage::Ptr iPtr(new SwwGeom::FloatImage(pixels,_width,_height,numChannels(),1));

        switch(type){
        case(SAVE_AS_FLOAT):
        case(SAVE_AS_IS):
            break;
        case(SAVE_AS_HALF):
            typeDesc = OIIO_NAMESPACE::TypeDesc::HALF;
            break;
        case(SAVE_AS_UINT8):
            typeDesc = OIIO_NAMESPACE::TypeDesc::UCHAR;
            break;
        }

        if(!iPtr->saveImage(fileName,true,typeDesc))return false;
        break;
    }

    case (GL_RGBA8):
    case (GL_RGB8):
    case (GL_RGB):
    case (GL_RGBA):
    {
        OIIO_NAMESPACE::TypeDesc typeDesc = OIIO_NAMESPACE::TypeDesc::UCHAR;
        uint8_t* pixels = new uint8_t[_width * _height * numChannels()];
        glBindTexture( _target, _texId );
        glGetTexImage(_target,0,_format,_type,pixels);
        glBindTexture(  _target, 0);
        SwwGeom::ByteImage::Ptr iPtr(new SwwGeom::ByteImage(pixels,_width,_height,numChannels(),1));

        switch(type){
        case(SAVE_AS_FLOAT):
            typeDesc = OIIO_NAMESPACE::TypeDesc::FLOAT;
            break;
        case(SAVE_AS_HALF):
            typeDesc = OIIO_NAMESPACE::TypeDesc::HALF;
            break;
        case(SAVE_AS_UINT8):
        case(SAVE_AS_IS):
            break;
        }

        if(!iPtr->saveImage(fileName,true,typeDesc))return false;
        break;
    }
    default:
        SWW_ERR("unsupprted internal format:" << std::nouppercase << std::showbase << std::hex << _internalFormat);

    }//switch

    SWW_INF("save image. : " << fileName);

    return true;
}

bool GLTexture::saveROI(const char *fileName, int xOrigin, int yOrigin, int roiWidth, int roiHeight){
    OIIO_NAMESPACE_USING;

    if(xOrigin+roiWidth > _width || yOrigin+ roiHeight > _height){
        SWW_ERR("ROI out of field.");
        return false;
    }

    int channels = numChannels();
    if(channels == -1)return false;

    if(_internalFormat != GL_RGBA32F)
    {
        SWW_ERR("supported saving for only GL_RGBA32F floating texture .");
        return false;
    }

    //ディレクトリ作成
    if(!Sww::Util::IOUtility::makeDir(fileName)){
        return false;
    }

//    ImageOutput *out = ImageOutput::create (fileName);
    auto out = ImageOutput::create(fileName);
    if (! out) return false;

    //float 縛り
    float* pixels = new float[_width * _height * channels];

    glBindTexture( _target, _texId );
    glGetTexImage(_target,0,_format,_type,pixels);
    glBindTexture(  _target, 0);

    //拡張子でビット深度を決める---------------------------------------------------------
    QFileInfo info(fileName);
    QString suffix = info.completeSuffix();

    ImageSpec spec;
    spec.width = _width;
    spec.height = _height;
    spec.nchannels = channels;

    cv::Mat mat3channels;//tif用　ポインタ保持

    bool mustDeletePixels = true;
    if(suffix == "tif" ||suffix == "tiff")
    {
        cv::Mat mat(_height,_width,CV_32FC(channels),pixels);
        //cv::Mat mat3channels;
        cv::cvtColor(mat,mat3channels,cv::COLOR_RGBA2RGB);
        //cv::pow(mat3channels,1,matSwwt);
        delete[] pixels;
        mustDeletePixels = false; //所有権はcv::matにあるのでdelete不要
        pixels = mat3channels.ptr<float>(0);
        spec.width = mat3channels.cols;
        spec.height = mat3channels.rows;
        spec.nchannels = mat3channels.channels();
        channels = mat3channels.channels();

        spec.format = TypeDesc::UCHAR;
        spec.attribute("compression","lzw");
        spec.attribute ("oiio:ColorSpace", "sRGB");
    }else if(suffix == "exr")
    {
        spec.format = TypeDesc::HALF;
        spec.attribute("compression","zip");
    }else
    {
        spec.format = TypeDesc::UCHAR;
    }

    //----------------------------------------------------------------------------

    float* pixelsRoi = new float[roiWidth * roiHeight * channels];
    for(int y = 0;y < roiHeight;++y){
        for(int x = 0;x < roiWidth;++x){
            for(int c = 0;c < channels;++c){
                pixelsRoi[y * roiWidth * channels + x * channels + c] =
                        pixels[(y + yOrigin)  * _width * channels + (x + xOrigin) * channels + c];
            }
        }
    }

    spec.width = roiWidth;
    spec.height = roiHeight;

    if(! out->open (fileName, spec)){
        if(mustDeletePixels)delete [] pixels;
        return false;
    }

    //書き出し 上下フリップして書き出す
    int scanlineSize = roiWidth * channels;

    //ＧＬ側のフォーマットをoiio式で渡す
    TypeDesc oiioInternalFormat = glInternalFormatToOiioTypeDesc(_internalFormat);
    if(oiioInternalFormat == TypeDesc::UNKNOWN){
        if(mustDeletePixels) delete [] pixels;
        return false;
    }

    bool result = out->write_image(oiioInternalFormat,
                                   pixelsRoi + ((roiHeight-1) * scanlineSize),//先頭ピクセルのポインタ
                                   sizeof(float) * channels, //X stride.
                                   -scanlineSize * sizeof(float));//Y stride.


    out->close();

    delete [] pixelsRoi;

    if(mustDeletePixels) delete [] pixels;
    if(!result) return false;
    std::cout << "save tex:" << fileName << std::endl;
    return true;
}

#endif

int GLTexture::numChannels()const
{
    switch(_format)
    {
    case GL_RGBA:
        return 4;
    case GL_RGB:
        return 3;
    case GL_RG:
        return 2;
    case GL_DEPTH_COMPONENT:
        return 1;
        //    case GL_DEPTH_COMPONENT32F:
        //        return 1;
    case GL_LUMINANCE:
        return 1;
    case GL_RED:
        return 1;
    default:
        SWW_ERR("numChannels case statemaent not defined." << " format:" << _format);
        return -1;
    }
}

void GLTexture::bindUniform(GLuint programId,const std::string& varName,int texunit) const
{
    //_mutex.lock();

    glActiveTexture(GL_TEXTURE0 + texunit);
    bind();
#if 0
    GLint linked;
    glGetProgramiv(programId, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        SWW_ERR("setTextureUnit needs program to be linked.");
        return;
    }
#endif
    GLint id = glGetUniformLocation(programId,varName.c_str());
    if (id == -1) {
        SWW_ERR("get location failed. " << varName);
        //_mutex.unlock();
        return;
    }

    //    GLint whichID;
    //    glGetIntegerv(GL_ACTIVE_TEXTURE, &whichID);
    //    if(whichID - 33984 != texunit){
    //        SWW_ERR("invalid tex unit");
    //    }

    glUniform1i(id, texunit);
    glActiveTexture(GL_TEXTURE0);

    //_mutex.unlock();
}

void GLTexture::drawQuad(){
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode( GL_PROJECTION );
    //glPopMatrix();
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    float color[4] = {1,1,1,1};
    bind();

    glColor4fv (color);
    glBegin(GL_QUADS);
    {
        glTexCoord2d(0.0, 0.0);
        glVertex2f(0.0, 0.0);
        glTexCoord2d(1.0, 0.0);
        glVertex2f(1.0, 0.0);
        glTexCoord2d(1.0, 1.0);
        glVertex2f(1.0, 1.0);
        glTexCoord2d(0.0, 1.0);
        glVertex2f(0.0, 1.0);
    }
    glEnd();
    release();

    glPopMatrix();
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopAttrib();


}

}//namespace
