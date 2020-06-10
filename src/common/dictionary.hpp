#pragma once

#include "common.hpp"
#include "common_serialization.hpp"
#include "concepts.hpp"
#include "serialization.hpp"
#include "vb_io.hpp"

#include <string>
#include <string_view>

namespace ir::common {
struct Dictionary {
    struct Element {
        size_t pos, len;

        bool operator==(const Element& other) {
            return pos == other.pos && len == other.len;
        }
    };
    std::string dic;

    std::string_view get(size_t pos, size_t len) const {
        return std::string_view(dic).substr(pos, len);
    };

    std::string_view get(const Element &e) const {
        return get(e.pos, e.len);
    }
};

template <>
struct Serialization<Dictionary::Element> {
    static void serialize(std::ofstream &fout, const Dictionary::Element &a) {
        Serialization<size_t>::serialize(fout, a.pos);
        Serialization<size_t>::serialize(fout, a.len);
    }
    static Dictionary::Element deserialize(std::ifstream &fin) {
        Dictionary::Element ele;
        ele.pos = Serialization<size_t>::deserialize(fin);
        ele.len = Serialization<size_t>::deserialize(fin);
        return ele;
    }
};

template <>
struct Serialization<Dictionary> {
    static void serialize(std::ofstream &fout, const Dictionary &a) {
        Serialization<std::string>::serialize(fout, a.dic);
    }
    static Dictionary deserialize(std::ifstream &fin) {
        Dictionary ret;
        ret.dic = Serialization<std::string>::deserialize(fin);
        return ret;
    }
};

} // namespace ir::common

namespace std {
template <>
struct hash<ir::common::Dictionary::Element> {
    size_t operator()(const ir::common::Dictionary::Element &e) const {
        size_t seed = 0;
        ir::common::hash_combine(seed, e.pos);
        ir::common::hash_combine(seed, e.len);
        return seed;
    }
};
} // namespace std