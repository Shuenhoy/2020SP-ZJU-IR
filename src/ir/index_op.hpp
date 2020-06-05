#pragma once

#include <common/common.hpp>
#include <common/concepts.hpp>

#include <algorithm>
#include <vector>

namespace ir::ir {

template <typename T>
requires common::Equal<T> struct IndexOp {
    std::vector<T> index_union(const std::vector<T> &a, const std::vector<T> &b) {
        std::vector<T> output;
        std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(output));
        return output;
    }
    std::vector<T> index_intersection(const std::vector<T> &a, const std::vector<T> &b) {
        std::vector<T> output;
        std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(output));
        return output;
    }

    std::vector<T> index_difference(const std::vector<T> &a, const std::vector<T> &b) {
        std::vector<T> output;
        std::set_difference(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(output));
        return output;
    }
};

} // namespace ir::ir
