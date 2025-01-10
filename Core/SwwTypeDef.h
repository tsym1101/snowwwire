#ifndef SWWTYPEDEF_H
#define SWWTYPEDEF_H

#include <vector>
#include <memory> // std::shared_ptr
#include <ImathVec.h>
#include <ImathMatrix.h>
#include "core/SwwNsCore.h"

SWW_CORE_NAMESPACE_BEGIN

class NodeBase;
class Node;
class PlugBase;
// class NodeGraph;
struct PlugMeta;

// typedef std::shared_ptr<NodeBase> NodeBasePtr;
// typedef std::shared_ptr<PlugBase> PlugBasePtr;
// typedef std::shared_ptr<Node> NodePtr;
// typedef std::shared_ptr<NodeGraph> NodeGraphPtr;
typedef std::shared_ptr<PlugMeta> PlugMetaPtr;

typedef std::vector<std::string> StringArray;
typedef std::vector<const std::string> ConstStringArray;
typedef std::shared_ptr<StringArray> StringArrayPtr;
typedef std::shared_ptr<const StringArray> StringArrayConstPtr;

typedef std::vector<float> FloatArray;
typedef std::shared_ptr<FloatArray> FloatArrayPtr;
typedef std::shared_ptr<const FloatArray> FloatArrayConstPtr;
typedef std::vector<Imath::V2f> V2fArray;
typedef std::shared_ptr<V2fArray> V2fArrayPtr;
typedef std::shared_ptr<const V2fArray> V2fArrayConstPtr;
typedef std::vector<Imath::V3f> V3fArray;
typedef std::shared_ptr<V3fArray> V3fArrayPtr;
typedef std::shared_ptr<const V3fArray> V3fArrayConstPtr;
typedef std::vector<Imath::V4f> V4fArray;
typedef std::shared_ptr<V4fArray> V4fArrayPtr;
typedef std::shared_ptr<const V4fArray> V4fArrayConstPtr;
typedef std::vector<Imath::V3d> V3dArray;
typedef std::shared_ptr<V3dArray> V3dArrayPtr;
typedef std::shared_ptr<const V3dArray> V3dArrayConstPtr;
typedef std::vector<Imath::V4d> V4dArray;
typedef std::shared_ptr<V4dArray> V4dArrayPtr;
typedef std::shared_ptr<const V4dArray> V4dArrayConstPtr;
typedef std::vector<int> IntArray;
typedef std::shared_ptr<IntArray> IntArrayPtr;
typedef std::shared_ptr<const IntArray> IntArrayConstPtr;
typedef std::vector<uint32_t> Uint32Array;
typedef std::shared_ptr<Uint32Array> Uint32ArrayPtr;
typedef std::shared_ptr<const Uint32Array> Uint32ArrayConstPtr;
typedef std::vector<uint8_t> Uint8Array;
typedef std::shared_ptr<Uint8Array> Uint8ArrayPtr;
typedef std::shared_ptr<const Uint8Array> Uint8ArrayConstPtr;
typedef std::vector<Imath::M44f> M44fArray;
typedef std::shared_ptr<M44fArray> M44fArrayPtr;
typedef std::shared_ptr<const M44fArray> M44fArrayConstPtr;

typedef Imath::Vec4<uint8_t> V4uc;

SWW_CORE_NAMESPACE_END

#endif // SWWTYPEDEF_H
