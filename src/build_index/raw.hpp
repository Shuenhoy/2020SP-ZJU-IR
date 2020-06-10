#pragma once

#include <common/common.hpp>
#include <common/dictionary.hpp>
#include <common/index.hpp>
#include <tuple>

namespace ir::build_index {

template <typename T>
using Raw = std::unordered_map<std::string, std::vector<T>>;

template <typename T>
inline std::pair<common::Index<common::Dictionary::Element, T>, common::Dictionary> raw_to_fi(const Raw<T> &raw) {
    NOT_IMPLEMENTED;
}
} // namespace ir::build_index
