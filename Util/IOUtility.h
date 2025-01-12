#ifndef IOUTILITY_H
#define IOUTILITY_H

#include "Util/UtilAPI.h"
#include <QString>

#ifdef USE_OIIO
#include <OpenImageIO/version.h>
#include <OpenImageIO/typedesc.h>
#endif
#include "Core/SwwAssert.h"
#include <QFile>

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#else
#include <QDataStream>
#endif

//class half;
#include <Imath/ImathConfig.h>
namespace IMATH_INTERNAL_NAMESPACE {
    class half;
}
using half = IMATH_INTERNAL_NAMESPACE::half;
#ifdef USE_OIIO
OIIO_NAMESPACE_BEGIN
    class ImageSpec;
OIIO_NAMESPACE_END
#endif

SWW_UTIL_NAMESPACE_BEGIN
class SWWUTILSHARED_EXPORT IOUtility
{
public:
    static std::string replaceFileNumber(int frame,const std::string &fileName);
    static std::string makeSeaqenceName(const std::string &fileName);//aaa_####.tifみたいな形式を作る

    static std::string replaceCameraName(int num, const std::string &fileName);
    static bool makeDir(const std::string &fileName);

    #ifdef USE_OIIO
    static bool saveImage(const std::string &fileName,float* pixels,  int width, int height, int channels, bool flip = false,
                          OIIO_NAMESPACE::TypeDesc desc = OIIO_NAMESPACE::TypeDesc::FLOAT);
    static bool saveImage( const std::string &fileName,unsigned char* pixels, int width, int height, int channels, bool flip = false,
                          OIIO_NAMESPACE::TypeDesc desc = OIIO_NAMESPACE::TypeDesc::UCHAR);
    static bool saveImage(const std::string &fileName,half* pixels,  int width, int height, int channels, bool flip = false,
                          OIIO_NAMESPACE::TypeDesc desc = OIIO_NAMESPACE::TypeDesc::HALF);

    static bool loadImage(const std::string &fileName, float*& pixels, int& width, int& height, int &channels, bool flip = false);
    static bool loadImage(const std::string &fileName, unsigned char*& pixels, int& width, int& height, int &channels, bool flip = false);
    static bool loadImage(const std::string &fileName, half*& pixels, int& width, int& height, int &channels, bool flip = false);

    static bool initOiio();

    static bool getImageSpec(const std::string& fileName, OIIO_NAMESPACE::ImageSpec* spec);

    template <class T>
    static OIIO_NAMESPACE::TypeDesc getOiioTypeDesc(){
        OIIO_NAMESPACE_USING;
        if(typeid(T) == typeid(float)){
            return  TypeDesc::FLOAT;
        }else if(typeid(T) == typeid(uchar)){
            return TypeDesc::UCHAR;
        }else if(typeid(T) == typeid(half)){
            return TypeDesc::HALF;
        }else{
            return TypeDesc::UNKNOWN;
        }
    }
#endif

    template <class T>
//    static bool loadPlaneBin(const std::string &fileName, boost::shared_array<T> &array, size_t &numElements, int devide);
    static bool loadPlaneBin(const std::string &fileName, std::unique_ptr<T[]> &array, size_t &numElements, int devide);
    static float reverseFloat( const float inFloat );
    template<typename T>
    static T swapByte(const T value);
    static bool isLittleEndian();

private:
    static std::string replaceFileNumberStd(int frame, const std::string &fileName); //sprintfを用いる

};

template <class T>
bool IOUtility::loadPlaneBin(const std::string &fileName, std::unique_ptr<T[]> &array, size_t &numElements, int devide) {
    SWW_DEBUG("load bin : " << fileName);

    QFile fileIn1(fileName.c_str());

    if (!fileIn1.open(QIODevice::ReadOnly)) {
        SWW_ERR("err. read file failed. " << fileName);
        return false;
    }

    size_t fileSize = fileIn1.size();
    numElements = fileSize / sizeof(T);
    array = std::make_unique<T[]>(numElements);
    QDataStream in1(&fileIn1);

    std::cout << "read " << fileName << std::endl;

    size_t fileBlockSize = fileSize / devide;
    size_t fileBlockSizeRemainder = fileSize % devide;

    size_t total = 0;
    for (size_t i = 0; i < devide; i++) {
        int result = in1.readRawData(reinterpret_cast<char*>(array.get()) + (fileBlockSize * i), fileBlockSize);
        if (result == -1) {
            SWW_ERR("read raw data failed.");
            return false;
        }
        total += result;
    }

    if (fileBlockSizeRemainder != 0) {
        int result = in1.readRawData(reinterpret_cast<char*>(array.get()) + (fileBlockSize * devide), fileBlockSizeRemainder);
        if (result == -1) {
            std::cout << "read raw data failed." << std::endl;
            return false;
        }
        total += result;
    }

    SWW_DEBUG_VALUE(total);
    SWW_DEBUG_VALUE(fileSize);
    SWW_DEBUG_VALUE(numElements);

    fileIn1.close();
    return true;
}

inline float IOUtility::reverseFloat( const float inFloat )
{
    float retVal;
    char *floatToConvert = ( char* ) & inFloat;
    char *returnFloat = ( char* ) & retVal;

    // swap the bytes into a temporary buffer
    returnFloat[0] = floatToConvert[3];
    returnFloat[1] = floatToConvert[2];
    returnFloat[2] = floatToConvert[1];
    returnFloat[3] = floatToConvert[0];

    return retVal;
}

template<typename T>
T IOUtility::swapByte(const T value){
    T result;
    char *buffer = ( char* ) & value;
    char *returnBuffer = ( char* ) & result;
    unsigned char byteSize = sizeof(T);
    for(unsigned char i = 0;i < byteSize;++i){
        returnBuffer[i] = buffer[byteSize - 1 - i];
    }
    return result;
}

inline bool IOUtility::isLittleEndian(){
    int x=1; // 0x00000001
    if (*(char*)&x) {
        SWW_DEBUG("This system is little endian.");
        return true;
    }else{
        SWW_DEBUG("This system is big endian.");
        return false;
    }
}

SWW_UTIL_NAMESPACE_END

#endif // IOUTILITY_H
