#pragma once

#include <fstream>
#include <unordered_map>

#include "common.hpp"
#include "concepts.hpp"

namespace ir::common {

template <typename K, typename C, typename V>
requires Serializable<K> &&Hashable<K> &&Serializable<V> &&CompareWith<K, C> struct Index {
    std::unordered_map<K, std::vector<V>> index;
    std::vector<decltype(index.begin())> items;
    void serialize(std::ofstream &fout) {
        NOT_IMPLEMENTED;
    }
    static Index deserialize(std::ifstream &fin) {
        NOT_IMPLEMENTED;
    }
};

} // namespace ir::common