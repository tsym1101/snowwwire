#ifndef SWW_IMAGE_H
#define SWW_IMAGE_H

#include <memory> // std::unique_ptr
#include <half.h>
#include <Core/SwwMacro.h>
#include "Util/IOUtility.h"
#include "Core/Plug.h"

namespace SwwGeom {

template <class BIT_DEPTH_T>
class Image {
public:

    typedef BIT_DEPTH_T thisType;
    typedef Image<BIT_DEPTH_T> thisImageType;
    typedef std::shared_ptr<thisImageType> Ptr;
    typedef std::shared_ptr<const thisImageType> ConstPtr;

    Image() : _width(1), _height(1), _depth(1), _channels(1) {}
    Image(BIT_DEPTH_T* pixels, int width, int height, int channels, int depth)
        : _width(width), _height(height), _depth(depth), _channels(channels) {
        _pixels.reset(pixels);
    }

    std::shared_ptr<Image<BIT_DEPTH_T>> deepCopy() const {
        std::shared_ptr<Image<BIT_DEPTH_T>> result(new Image<BIT_DEPTH_T>());
        *(result.get()) = *this;
        size_t memSize = _width * _height * _depth * _channels;
        result->_pixels = std::make_unique<BIT_DEPTH_T[]>(memSize);
        memcpy(result->_pixels.get(), _pixels.get(), memSize * sizeof(BIT_DEPTH_T));
        return result;
    }

    bool saveImage(const std::string& fileName, bool flip = false,
                   OIIO_NAMESPACE::TypeDesc desc = Sww::Util::IOUtility::getOiioTypeDesc<BIT_DEPTH_T>()) const {
        BIT_DEPTH_T* pixels = _pixels.get();
        if (!pixels) {
            SWW_ERR("invalid pixels." << fileName);
            return false;
        }
        return Sww::Util::IOUtility::saveImage(fileName, pixels, _width, _height, _channels, flip, desc);
    }

    bool loadImage(const std::string& fileName, bool flip = false) {
        BIT_DEPTH_T* pixels = nullptr;
        bool valid = Sww::Util::IOUtility::loadImage(fileName, pixels, _width, _height, _channels, flip);
        if (!valid) {
            SWW_ERR("load image failed. " << fileName);
            return false;
        }
        _pixels.reset(pixels);
        return true;
    }

    const BIT_DEPTH_T* pixels() const { return _pixels.get(); }
    BIT_DEPTH_T* pixels() { return _pixels.get(); }
    void pixels(std::unique_ptr<BIT_DEPTH_T[]> p) { _pixels = std::move(p); }

    int getNumDimension() const {
        if (_depth == 1) {
            if (_height == 1)
                return 1;
            else
                return 2;
        } else
            return 3;
    }

private:
    std::unique_ptr<BIT_DEPTH_T[]> _pixels;

    SWW_SIMPLE_MEMBER(int, width)
    SWW_SIMPLE_MEMBER(int, height)
    SWW_SIMPLE_MEMBER(int, depth)
    SWW_SIMPLE_MEMBER(int, channels)
};

typedef Image<float> FloatImage;
typedef Image<unsigned char> ByteImage;
typedef Image<half> HalfImage;

} // namespace SwwGeom

#endif // SWW_IMAGE_H
