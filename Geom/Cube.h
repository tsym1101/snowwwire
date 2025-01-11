#ifndef SWW_CUBE_H
#define SWW_CUBE_H

#include "Geom/Mesh.h"

namespace SwwGeom {

class SWWGEOMSHARED_EXPORT Cube : public Mesh
{
public:
    typedef std::shared_ptr<Cube> Ptr;
    typedef std::shared_ptr<const Cube> ConstPtr;
    Cube(const std::string &name);
};

}//ns

#endif // SWW_CUBE_H
