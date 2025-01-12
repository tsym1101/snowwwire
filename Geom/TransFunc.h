#ifndef SWW_TRANSFUNC_H
#define SWW_TRANSFUNC_H

#include <vector>
#include <string>
#include <ImathVec.h>
#include "Geom/GeomAPI.h"

namespace SwwGeom {

class  SWWGEOMSHARED_EXPORT TransFunc {
public:
    //    TransFunc(int width = 256, int height = 1, int depth = 1,
    //        GLint format = GL_RGBA, GLenum dataType = GL_UNSIGNED_BYTE,
    //              GLenum texTarget = GL_TEXTURE_1D);
    TransFunc(int width = 256, int height = 1, int depth = 1);
    virtual ~TransFunc();
    virtual std::string getShaderDefines() const;
    virtual std::string getSamplerType() const;
    const Imath::V3i& getDimensions() const;

    virtual void resize(int width, int height = 1, int depth = 1);

    bool mustResetTexture()const;

    virtual int getNumDimensions() const;
    virtual Imath::V2f getDomain(int /*dimension = 0*/) const { return Imath::V2f(0.0f, 1.0f); }
    virtual void setDomain(Imath::V2f /*domain*/, int /*dimension = 0*/) {}
    virtual void setDomain(float lower, float upper, int dimension) { setDomain(Imath::V2f(lower, upper), dimension); }

protected:
    Imath::V3i dimensions_;       ///< dimensions of the transfer function texture
    bool _mustResetTexture;

private:

};

} // ns

#endif // SWW_TRANSFUNC_H

