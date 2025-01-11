#ifndef DUALMAP_H
#define DUALMAP_H

#include <vector>
#include <map>
#include <QMap>
#include "Core/VectorHelper.h"

#include <qglobal.h>

SWW_CORE_NAMESPACE_BEGIN

//1 NAMED_PTR_TYPEはgetName()const を持っているポインタ型
//2 vector,mapともに重複は認めない
template <class NAMED_PTR_TYPE>
class  DualMap{

public:
    typedef std::vector<NAMED_PTR_TYPE> thisVec;
    //typedef std::map<std::string,NAMED_PTR_TYPE> thisMap;
    typedef QMap<std::string,NAMED_PTR_TYPE> thisMap;

    void push_back(const NAMED_PTR_TYPE &value);
    void clear();

    void erase(const std::string& key);
    size_t size()const;

    typename thisVec::iterator begin(){return _vec.begin();}
    typename thisVec::iterator end(){return _vec.end();}
    typename thisVec::const_iterator constBegin()const{return _vec.begin();}
    typename thisVec::const_iterator constEnd()const{return _vec.end();}

    typename thisMap::iterator mapBegin(){return _map.begin();}
    typename thisMap::iterator mapEnd(){return _map.end();}
    typename thisMap::const_iterator mapConstBegin()const{return _map.begin();}
    typename thisMap::const_iterator mapConstEnd()const{return _map.end();}

    const NAMED_PTR_TYPE& operator[](size_t i) const{return _vec[i];}
    NAMED_PTR_TYPE& operator[](size_t i){return _vec[i];}
    //const NAMED_PTR_TYPE operator[](const std::string& key) const{return _map.value(key);}
    //NAMED_PTR_TYPE& operator[](const std::string& key){return _map.value(key);}//ない場合は例外をスロー

    bool empty()const{return _vec.empty();}

    bool contains(const std::string& key) const;

    const NAMED_PTR_TYPE value(const std::string& key) const;
    //typename  NAMED_PTR_TYPE& value(const std::string& key); //ない場合は例外をスロー

    std::vector<std::string> keys()const;

    const thisVec& vec()const{return _vec;}

private:
    thisVec _vec;
    thisMap _map;
};

template<class NAMED_PTR_TYPE>
inline void DualMap<NAMED_PTR_TYPE>::push_back(const NAMED_PTR_TYPE &value)
{
    const std::string& key = value->getName();

    //Q_ASSERT_X(!mapContains(_map,key),key.c_str(),"map already contain.");
    Q_ASSERT_X(!_map.contains(key),key.c_str(),"map already contain.");
    Q_ASSERT_X(!vectorContains(_vec,value),key.c_str()," vec already contain.");

    _vec.push_back(value);
    _map[key] = value;
}

template<class NAMED_PTR_TYPE>
inline void DualMap<NAMED_PTR_TYPE>::clear()
{
    _vec.clear();
    _map.clear();
}

template<class NAMED_PTR_TYPE>
inline void DualMap<NAMED_PTR_TYPE>::erase(const std::string&  key)
{
    const NAMED_PTR_TYPE& value = _map.value(key);
    vectorErase(_vec,value);
   //_map.erase(key);
   _map.remove(key);
}

template<class NAMED_PTR_TYPE>
inline size_t DualMap<NAMED_PTR_TYPE>::size() const
{
    return _vec.size();
}

template<class NAMED_PTR_TYPE>
inline bool DualMap<NAMED_PTR_TYPE>::contains(const std::string &key)const
{
    //return mapContains(_map,key);
    return _map.contains(key);
}

template<class NAMED_PTR_TYPE>
const typename NAMED_PTR_TYPE DualMap<NAMED_PTR_TYPE>::value(const std::string& key) const
{
    //return _map.at(key);
    return _map.value(key);
}

//template<class NAMED_PTR_TYPE>
//typename  NAMED_PTR_TYPE& DualMap<NAMED_PTR_TYPE>::value(const std::string& key)
//{
//    //return _map.at(key);
//    //Q_ASSERT_X(_map.contains(key),"value().","map not contains.");
//    if(!_map.contains(key)){
//        throw "Exception : map not contains.\n";;
//    }
//    return _map[key];//releaseモードでの挙動に注意　要素が追加されてしまう
//}

template<class NAMED_PTR_TYPE>
std::vector<std::string> DualMap<NAMED_PTR_TYPE>::keys()const
{
    std::vector<std::string> keys;
//    for(auto& kv:_map){
//        keys.push_back(kv.first);
//    }

    Q_FOREACH(std::string key,_map.keys()){
        keys.push_back(key);
    }

    return keys;
}

SWW_CORE_NAMESPACE_END

#endif // DUALMAP_H
