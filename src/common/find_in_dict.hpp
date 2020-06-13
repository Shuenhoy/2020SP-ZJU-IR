#pragma once

#include "common.hpp"
#include "dictionary.hpp"
#include "index.hpp"

#include <string_view>

namespace ir::common {
template <typename T>
inline auto find_in_dict(std::string_view tofind, const Dictionary &dict,
                         const Index<Dictionary::Element, T> &index) {
    auto iter = common::binary_search(index.items.begin(), index.items.end(), [&](auto &&ele) {
        return dict.get(ele).compare(tofind) < 0;
    });
    return dict.get(*iter).compare(tofind) == 0 ? iter : index.items.end();
}
} // namespace ir::common
