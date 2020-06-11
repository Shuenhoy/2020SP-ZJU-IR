#pragma once

#include "common.hpp"
#include "dictionary.hpp"
#include "index.hpp"

#include <cassert>
#include <string_view>
#include <vector>

#include "find_in_dict.hpp"

namespace ir::common {

using KGramInvIndex = Index<Dictionary::Element, Dictionary::Element>;

inline std::vector<Dictionary::Element> kgram(std::string_view token, size_t k,
                                              const Dictionary &kgram_dict,
                                              const KGramInvIndex &kgram_index) {
    std::vector<Dictionary::Element> ret;
    for (auto i = 0; i < token.size() - k; i) {
        auto x = find_in_dict<Dictionary::Element>(
            token.substr(i, k), kgram_dict, kgram_index);
        assert(x != kgram_index.items.end());
        ret.push_back(*x);
    }
    return ret;
}
} // namespace ir::common
