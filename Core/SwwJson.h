#ifndef SWWJSON_H
#define SWWJSON_H

#include <nlohmann/json.hpp>
#include <nlohmann/fifo_map.hpp>

// A workaround to give to use fifo_map as map, we are just ignoring the 'less' compare
template<class K, class V, class dummy_compare, class A>
using workaround_fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;
using SwwJson = nlohmann::basic_json<workaround_fifo_map>; //挿入順を保持するコンテナを使用

#endif // SWWJSON_H
