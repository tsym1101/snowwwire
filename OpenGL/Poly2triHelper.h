#ifndef POLY2TRIHELPER_H
#define POLY2TRIHELPER_H

#include <ImathVec.h>
#include "Core/SwwTypeDef.h"
#include <memory> // std::unique_ptr, std::shared_ptr

#include "Core/SwwMacro.h"
#include "OpenGL/OpenGLAPI.h"

namespace p2t {
class CDT;
struct Point;
}

namespace SwwOpenGL {

class SWWOPENGLSHARED_EXPORT Poly2triHelper
{
public:
    Poly2triHelper(Sww::Core::V2fArray& verts);

    bool getTriangles(Sww::Core::V2fArray& oPosArray,
                      Sww::Core::Uint32Array& oIndiceArray);

private:
    typedef std::shared_ptr<p2t::Point> PointPtr;
    typedef std::unique_ptr<p2t::CDT> CdtPtr;
    CdtPtr _cdtPtr;
    std::vector<p2t::Point*> _polyLine;
    std::vector<PointPtr> _pointPtrVec;
    Sww::Core::V2fArray& _org;

    SWW_SIMPLE_MEMBER_READ_ONLY(uint32_t, numVertex)
    SWW_SIMPLE_MEMBER_READ_ONLY(uint32_t, numTriangles)
};

} // namespace SwwOpenGL

#endif // POLY2TRIHELPER_H
