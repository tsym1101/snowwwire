#ifndef KNOBTYPEDESC_H
#define KNOBTYPEDESC_H

//typedef unsigned int SwwKnobTypeDesc;
using SwwKnobTypeDesc = unsigned int;

namespace KnobTypeDesc
{
namespace FLOAT
{
enum{
    SPIN_BOX = 0,
    SLIDER = 1,
    userType = 100
};
}

namespace STRING
{
enum{
    INPUT_IMAGE = 0,
    OUTPUT_IMAGE,
    LINE_EDIT,
    COMBO_BOX,

    userType = 100
};
}

namespace STRING_ARRAY
{
enum{
    INPUT_FILES = 0,
    MULTI_LINE_EDIT = 1,
    userType = 100
};
}

namespace MULTI_FILE
{
enum{
    INPUT_FILES = 0,
    userType = 100
};
}


namespace INTEGER
{
enum{
    SPIN_BOX = 0,
    SLIDER = 1,
    COMBO_BOX = 50,

    userType = 100
};
}

namespace V2F
{
enum{
    SPIN_BOX = 0,
    RANGE_SLIDER = 1,

    userType = 100
};
}

}

#endif // KNOBTYPEDESC_H
