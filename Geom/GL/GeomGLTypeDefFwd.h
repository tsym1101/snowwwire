#ifndef SWWGEOMGLTYPEDEFFWD_H
#define SWWGEOMGLTYPEDEFFWD_H

#include <memory>

namespace SwwGeom {

class GLVertexBufferObject;
class GLIndexBufferObject;
class GLMesh;
class GLPoints;
class GLLines;
class GLInstance;

typedef std::shared_ptr<GLVertexBufferObject> GLVertexBufferObjectPtr;
typedef std::shared_ptr<GLIndexBufferObject> GLIndexBufferObjectPtr;
typedef std::shared_ptr<GLMesh> GLMeshPtr;
typedef std::shared_ptr<GLPoints> GLPointsPtr;
typedef std::shared_ptr<GLLines> GLLinesPtr;
typedef std::shared_ptr<GLInstance> GLInstancePtr;

}

#endif // SWWGEOMGLTYPEDEFFWD_H
