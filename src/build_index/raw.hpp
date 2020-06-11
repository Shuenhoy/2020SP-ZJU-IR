#pragma once

#include <common/common.hpp>
#include <common/dictionary.hpp>
#include <common/index.hpp>

#include <map>
#include <tuple>

namespace ir::build_index {

template <typename T>
using Raw = std::map<std::string, std::vector<T>>;

template <typename T>
inline std::pair<common::Index<common::Dictionary::Element, T>, common::Dictionary> raw_to_fi(const Raw<T> &raw) {
    common::Index<common::Dictionary::Element, T> index;
    common::Dictionary dict;
    for (auto &&[k, v] : raw) {
        common::Dictionary::Element ele;
        ele.len = k.size();
        ele.pos = dict.dic.size();
        dict.dic.append(k);
        index.items.push_back(ele);
        index.index[ele] = v;
    }
    return {index, dict};
}
} // namespace ir::build_index
