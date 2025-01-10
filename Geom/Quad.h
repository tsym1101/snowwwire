#ifndef SWWQUAD_H
#define SWWQUAD_H

#include "Geom/Mesh.h"

namespace SwwGeom {

class SWWGEOMSHARED_EXPORT Quad : public Mesh
{
public:
    typedef std::shared_ptr<Quad> Ptr;
    typedef std::shared_ptr<const Quad> ConstPtr;
    Quad(const std::string& name = "quad");
};

}//ns

#endif // SWWQUAD_H
