#pragma once

#include <string_view>
#include <unordered_map>

#include "common.hpp"
#include "dictionary.hpp"

namespace ir::common {

bool wildcard_match(std::string_view str, std::string_view pattern) {
    std::unordered_map<std::pair<size_t, size_t>, bool> memo;

    auto wild = [&](size_t i, size_t j, auto &&rec) mutable -> bool {
        if (memo.contains({i, j}))
            return memo[{i, j}];
        if (j == pattern.size())
            return i == str.size();
        bool ans = false;
        if (pattern[j] == '*') {
            ans = rec(i, j + 1, rec) || i < str.size() && rec(i + 1, j, rec);
        } else {
            ans = i < str.size() && pattern[j] == str[i] && rec(i + 1, j + 1, rec);
        }
        memo[{i, j}] = ans;
        return ans;
    };
    return wild(0, 0, wild);
}
} // namespace ir::common
