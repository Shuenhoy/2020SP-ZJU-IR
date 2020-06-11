#pragma once

#include <string_view>
#include <unordered_map>

#include "common.hpp"
#include "dictionary.hpp"

namespace ir::common {

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const
    {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

bool wildcard_match(std::string_view str, std::string_view pattern) {
    std::unordered_map<std::pair<size_t, size_t>, bool, pair_hash> memo;

    auto wild = [&](size_t i, size_t j, auto &&rec) mutable -> bool {
        if (memo.contains({i, j}))
            return memo[{i, j}];
        if (j == pattern.size())
            return i == str.size();
        bool ans = false;
        if (pattern[j] == '*') {
            ans = rec(i, j + 1, rec) || rec(i + 1, j, rec);
        } else {
            ans = i < str.size() && pattern[j] == str[i] && rec(i + 1, j + 1, rec);
        }
        memo[{i, j}] = ans;
        return ans;
    };
    return wild(0, 0, wild);
}
} // namespace ir::common
