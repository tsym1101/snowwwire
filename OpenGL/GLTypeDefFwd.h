#ifndef GLTYPEDEFFWD_H
#define GLTYPEDEFFWD_H

#include <memory> // std::shared_ptr
#include <vector>

namespace SwwOpenGL {
class IGLUniform;
class GLShaderHelper;
class IGLNode;
class IGLNodePlug;
class NdGLLight;
class NdGLCamera;

typedef std::vector<IGLUniform*> GLUniformArray;
typedef std::shared_ptr<GLShaderHelper> GLShaderHelperPtr;
typedef std::shared_ptr<IGLNode> IGLNodePtr;

typedef IGLNode* IGLNodeRawPtr;
typedef NdGLLight* NdGLLightRawPtr;

} // namespace SwwOpenGL

namespace SwwOpenGL {
class GLShaderProgram;
class GLTexture;
class GLRenderTarget;
class GLFrameBufferObject;
class GLLegacyVbo;
class GLUniformBufferObject;
class GLUniformBufferObjectHelper;
class UboMeta;

typedef std::shared_ptr<GLShaderProgram> GLShaderProgramPtr;
typedef std::shared_ptr<GLTexture> GLTexturePtr;
typedef std::shared_ptr<GLRenderTarget> GLRenderTargetPtr;
typedef std::shared_ptr<GLFrameBufferObject> GLFrameBufferObjectPtr;
typedef std::shared_ptr<GLLegacyVbo> GLLegacyVboPtr;
typedef std::shared_ptr<GLUniformBufferObject> GLUniformBufferObjectPtr;
typedef std::shared_ptr<GLUniformBufferObjectHelper> GLUniformBufferObjectHelperPtr;
typedef std::shared_ptr<UboMeta> UboMetaPtr;
typedef std::vector<UboMeta> UboMetaArray;

} // namespace SwwOpenGL

#endif // GLTYPEDEFFWD_H
