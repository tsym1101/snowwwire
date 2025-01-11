#ifndef SWWMATH_H
#define SWWMATH_H

#include <limits>
#include <algorithm>
#include "SwwNsUtil.h"

#ifndef MIN
    #define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
    #define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif

SWW_UTIL_NAMESPACE_BEGIN
class SwwMath{
public:
    template <class T>
    static T radToDeg(T value){
        return value * 57.295779513082321;
    }

    template <class T>
    static T degToRad(T value){
        return value * .017453292519943296;
    }

    static double pi(){
        return 3.14159265358979323846264338327950288;
    }

    static float random(float x, float y){
        float high = MAX(x, y);
            float low = MIN(x, y);
            return std::max(low, (low + ((high - low) * rand() / float(RAND_MAX))) * (1.0f - std::numeric_limits<float>::epsilon()));
    }
};
SWW_UTIL_NAMESPACE_END

#endif // SWWMATH_H
