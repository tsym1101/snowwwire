#ifndef SWW_VECTORHELPER_H
#define SWW_VECTORHELPER_H

#include <algorithm>
#include <vector>
#include <map>

template <typename T>
void vectorErase(std::vector<T>& vec, const T& element)
{
    typename std::vector<T>::iterator i = std::find(vec.begin(), vec.end(), element);
    if (i != vec.end())
    {
        vec.erase(i);
    }
}

#define SWW_VECTOR_FOREACH(type,vec) \
    for(std::vector<type>::iterator i = vec.begin(); i != vec.end() ; ++i)

#define SWW_VECTOR_FOREACH_CONST(type,vec) \
    for(std::vector<type>::const_iterator i = vec.begin(); i != vec.end() ; ++i)

template <typename T>
int vectorIndexOf(std::vector<T>& vec,const T& element)
{
    typename std::vector<T>::iterator i = std::find(vec.begin(), vec.end(), element);
    if (i != vec.end ()){
        int index = std::distance(vec.begin(), i);
        return  index;
    }

    return  -1;
}

template <typename T>
bool vectorContains(std::vector<T>& vec, const T& element) //constポインタをelementにできなかった
{
    if(std::find(vec.begin(), vec.end(), element) != vec.end()) return true;
    return false;
}

template <typename key,typename T>
bool mapContains(const std::map<key,T>& map, const key& k)
{
    return map.find(k) != map.end();
}



#endif // SWW_VECTORHELPER_H
