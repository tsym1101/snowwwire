#ifndef SWW_ARRAYPROPERTY_H
#define SWW_ARRAYPROPERTY_H

#include <ImathVec.h>
#include <ImathMatrix.h>
#include <memory>
#include <vector>
#include "Geom/GeomAPI.h"

namespace SwwGeom {

class ArrayProperty {
public:
    typedef std::shared_ptr<ArrayProperty> Ptr;
    typedef std::shared_ptr<const ArrayProperty> ConstPtr;

    enum ArrayType {
        kFloat,
        kInt,
        kUint64,
        kV3f,
        kV4f,
        kM44f
    };

    ArrayProperty(const std::string& name) : _name(name) {}
    virtual ArrayType type() const = 0;

    const std::string& getName() const { return _name; }
    void setName(const std::string& name) { _name = name; }

    template <class PropertyT>
    static const typename PropertyT::Ptr arrayPropertyCast(const ArrayProperty::Ptr propPtr) {
        const ArrayProperty::Ptr tmp = propPtr; // 参照のままだとクラッシュする
        return std::dynamic_pointer_cast<PropertyT>(tmp);
    }

private:
    std::string _name;
};

template <typename T, ArrayProperty::ArrayType TypeT>
class ArrayPropertyT : public ArrayProperty {
public:
    typedef std::vector<T> ThisArray;
    typedef std::shared_ptr<ThisArray> ArrayPtr;
    typedef std::shared_ptr<ArrayPropertyT<T, TypeT>> Ptr;
    typedef std::shared_ptr<const ArrayPropertyT<T, TypeT>> ConstPtr;

    ArrayPropertyT(const std::string& name, ArrayPtr array = ArrayPtr())
        : ArrayProperty(name), _array(array) {}
    ArrayProperty::ArrayType type() const { return TypeT; }

    const ArrayPtr& getArray() const { return _array; }
    void setArray(const ArrayPtr& arrayPtr) const { _array = arrayPtr; }

    void push_back(const T& value) {
        Q_ASSERT(_array);
        _array->push_back(value);
    }

private:
    mutable ArrayPtr _array;
};

typedef ArrayPropertyT<float, ArrayProperty::kFloat> FloatArrayProperty;
typedef ArrayPropertyT<int, ArrayProperty::kInt> Int32ArrayProperty;
typedef ArrayPropertyT<uint64_t, ArrayProperty::kUint64> Uint64ArrayProperty;
typedef ArrayPropertyT<Imath::V3f, ArrayProperty::kV3f> V3fArrayProperty;
typedef ArrayPropertyT<Imath::V4f, ArrayProperty::kV4f> V4fArrayProperty;
typedef ArrayPropertyT<Imath::M44f, ArrayProperty::kM44f> M44fArrayProperty;

namespace ArrayPropertyCast {
const FloatArrayProperty::Ptr SWWGEOMSHARED_EXPORT toFloatArray(const ArrayProperty::Ptr& p);
const Int32ArrayProperty::Ptr SWWGEOMSHARED_EXPORT toInt32Array(const ArrayProperty::Ptr& p);
const Uint64ArrayProperty::Ptr SWWGEOMSHARED_EXPORT toUint64Array(const ArrayProperty::Ptr& p);
const V3fArrayProperty::Ptr SWWGEOMSHARED_EXPORT toV3fArray(const ArrayProperty::Ptr& p);
const V4fArrayProperty::Ptr SWWGEOMSHARED_EXPORT toV4fArray(const ArrayProperty::Ptr& p);
const M44fArrayProperty::Ptr SWWGEOMSHARED_EXPORT toM44fArray(const ArrayProperty::Ptr& p);
}

} // namespace SwwGeom

#endif // SWW_ARRAYPROPERTY_H
