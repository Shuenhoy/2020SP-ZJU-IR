#pragma once

#include <fstream>
#include <unordered_map>

#include "common.hpp"
#include "common_serialization.hpp"
#include "concepts.hpp"
#include "serialization.hpp"
#include "vb_io.hpp"

namespace ir::common {

template <typename K, typename V, typename Extra = size_t>
requires Serializable<K> &&Hashable<K> &&Serializable<V> &&Serializable<Extra> struct Index {
    std::unordered_map<K, std::vector<V>> index;
    std::vector<Extra> extra_info;
    std::vector<K> items;
    void serialize(std::ofstream &fout) {
        Serialization<std::vector<Extra>>::serialize(fout, extra_info);
        Serialization<size_t>::serialize(fout, items.size());
        for (auto iter : items) {
            Serialization<K>::serialize(fout, iter->first);
            Serialization<size_t>::serialize(fout, iter->second.size());
            for (auto &&vs : iter->second) {
                Serialization<V>::serialize(fout, vs);
            }
        }
    }
    static Index<K, V, Extra> deserialize(std::ifstream &fin) {

        Index<K, V, Extra> ret;

        ret.extra = Serialization<std::vector<Extra>>::deserialize(fin);

        size_t item_num = Serialization<size_t>::deserialize(fin);
        for (auto i = 0; i < item_num; i++) {
            K key = Serialization<K>::deserialize(fin);
            size_t n = Serialization<size_t>::deserialize(fin);
            std::vector<V> vs;
            for (auto j = 0; j < n; j++) {
                vs.push_back(Serialization<V>::deserialize(fin));
            }
            ret.items.push_back(ret.index.insert(key, std::move(vs)));
        }

        return ret;
    }
};

} // namespace ir::common