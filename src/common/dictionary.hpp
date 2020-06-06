#pragma once

#include "common.hpp"
#include "concepts.hpp"
#include "serialization.hpp"

#include <string>
#include <string_view>

namespace ir::common {
struct Dictionary {
    struct Element {
        size_t pos, len;
        const Dictionary &dict;
        std::string_view get() const {
            return dict.get(pos, len);
        }
        void serialize(std::ofstream &fout) {
            NOT_IMPLEMENTED;
        }
        static Element deserialize(std::ifstream &fin) {
            NOT_IMPLEMENTED;
        }
    };
    std::string dic;

    std::string_view get(size_t pos, size_t len) const {
        return std::string_view(dic).substr(pos, len);
    };
};

template <>
struct Serialization<Dictionary::Element> {
    static void serialize(std::ofstream &fout, const Dictionary::Element &a) {
        NOT_IMPLEMENTED;
    }
    static Dictionary::Element deserialize(std::ifstream &fin) {
        NOT_IMPLEMENTED;
    }
};

template <>
struct Comparison<Dictionary::Element, std::string_view> {
    static inline bool Compare(const Dictionary::Element &a, std::string_view b) {
        return a.get() < b;
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