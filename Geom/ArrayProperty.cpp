#include "ArrayProperty.h"
#include <QDebug>

namespace SwwGeom {

namespace ArrayPropertyCast {

const FloatArrayProperty::Ptr toFloatArray(const ArrayProperty::Ptr& p) {
    FloatArrayProperty::Ptr ptr = std::dynamic_pointer_cast<FloatArrayProperty>(p);
    return ptr;
}

const Int32ArrayProperty::Ptr toInt32Array(const ArrayProperty::Ptr& p) {
    Int32ArrayProperty::Ptr ptr = std::dynamic_pointer_cast<Int32ArrayProperty>(p);
    return ptr;
}

const Uint64ArrayProperty::Ptr toUint64Array(const ArrayProperty::Ptr& p) {
    Uint64ArrayProperty::Ptr ptr = std::dynamic_pointer_cast<Uint64ArrayProperty>(p);
    return ptr;
}

const V3fArrayProperty::Ptr toV3fArray(const ArrayProperty::Ptr& p) {
    V3fArrayProperty::Ptr ptr = std::dynamic_pointer_cast<V3fArrayProperty>(p);
    return ptr;
}

const V4fArrayProperty::Ptr toV4fArray(const ArrayProperty::Ptr& p) {
    V4fArrayProperty::Ptr ptr = std::dynamic_pointer_cast<V4fArrayProperty>(p);
    return ptr;
}

const M44fArrayProperty::Ptr toM44fArray(const ArrayProperty::Ptr& p) {
    M44fArrayProperty::Ptr ptr = std::dynamic_pointer_cast<M44fArrayProperty>(p);
    return ptr;
}

} // namespace ArrayPropertyCast

} // namespace SwwGeom
