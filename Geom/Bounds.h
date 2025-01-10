#ifndef SWW_BOUNSWW_H
#define SWW_BOUNSWW_H

#include <ImathBox.h>

#include "Core/SwwTypeDef.h"
#include <memory>
#include "Geom/GeomAPI.h"

namespace SwwGeom {

class SWWGEOMSHARED_EXPORT Bounds{
public:
//    typedef std::shared_ptr<Bounds> Ptr;
//    typedef std::shared_ptr<const Bounds> ConstPtr;
//    Bounds(){}
    virtual Imath::Box3f getBounds() const = 0;
//    virtual void setBounds(const Imath::Box3f& b);

//protected:
//    Imath::Box3f _bbox;
};

//inline  Imath::Box3f Bounds::getBounds() const{
//    return _bbox;
//}

//inline void Bounds::setBounds(const Imath::Box3f &b){
//    _bbox = b;
//}

}

#endif // SWW_BOUNSWW_H
