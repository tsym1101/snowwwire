#ifndef SWWTYPEDESC_H
#define SWWTYPEDESC_H

#include "core/SwwNsCore.h"

//typedef unsigned int SwwTypeDesc;
using SwwTypeDesc = unsigned int;

SWW_CORE_NAMESPACE_BEGIN

const SwwTypeDesc BOOL =  0x0001;
const SwwTypeDesc INT =  0x0002;
const SwwTypeDesc FLOAT =  0x0003;
const SwwTypeDesc DOUBLE =  0x0004;
const SwwTypeDesc V2I =  0x0005;
const SwwTypeDesc V3I =  0x0006;
const SwwTypeDesc V2F =  0x0007;
const SwwTypeDesc V3F =  0x0008;
const SwwTypeDesc V4F =  0x0009;
const SwwTypeDesc V3D =  0x000A;
const SwwTypeDesc M44F =  0x000B;
const SwwTypeDesc BOX3D =  0x000C;
const SwwTypeDesc _VOID =  0x000D;
const SwwTypeDesc _NULL =  0x000E;
const SwwTypeDesc STRING =  0x000F;
const SwwTypeDesc STRING_ARRAY =  0x0010;

const SwwTypeDesc FLOAT_ARRAY =  0x0011;
const SwwTypeDesc V2F_ARRAY =  0x0012;
const SwwTypeDesc V3F_ARRAY =  0x0013;
const SwwTypeDesc V4F_ARRAY =  0x0014;
const SwwTypeDesc INT_ARRAY =  0x0015;
const SwwTypeDesc UINT32_ARRAY =  0x0016;
const SwwTypeDesc UINT8_ARRAY =  0x0017;

const SwwTypeDesc M33F =  0x0018;

const SwwTypeDesc NODE_TYPE =  0x00FF0;
const SwwTypeDesc OPENGL_TYPE =  0x00FFF;

//enum SwwTypeDescEnum
//{
//    //BOOL,
//    INT,
//    FLOAT,
//    DOUBLE,
//    V2I,
//    V3I,
//    V3F,
//    V4F,
//    V3D,
//    M44F,
//    BOX3D,
//    _VOID,
//    _NULL,
//    STRING,
//    STRING_ARRAY,
//    NODE_TYPE = 500,
//    OPENGL_TYPE = 1000
//};

SWW_CORE_NAMESPACE_END

#endif // SWWTYPEDESC_H
