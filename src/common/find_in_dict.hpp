#pragma once

#include "common.hpp"
#include "dictionary.hpp"
#include "index.hpp"

#include <string_view>

namespace ir::common {
template <typename T, typename ForwardIt>
inline ForwardIt find_in_dict(std::string_view tofind, const Dictionary &dict,
                              const Index<Dictionary::Element, T> &index) {
    return common::binary_search(index.items.begin(), index.items.end(), [&](const Dictionary::Element &ele) {
        return dict.get(ele) < tofind;
    });
}
} // namespace ir::common
