#pragma once

#include "common.hpp"
#include <cctype>
#include <string>
#include <vector>

namespace ir::common {

inline bool is_token(const char& c, bool is_query) {
    return std::isalnum(c) || c == '_' || c == '-' || is_query && c == '*' || is_query && c == '"';
}

/* 将字符串变成词项，目前仅仅保留了字母、数字、下划线与连词符，其他符号全部去除 */
inline std::vector<std::string_view> tokenize(std::string_view input, bool is_query) {
    std::vector<std::string_view> tokens;
    for (size_t i = 0; i < input.size(); i++) {
        if (is_token(input[i], is_query)) {
            size_t begin = i;
            while (++i < input.size() && is_token(input[i], is_query))
                ;
            size_t length = i - begin;
            tokens.push_back(input.substr(begin, length));
        }
    }
    return tokens;
}

/* 词项归一化。步骤：(1) 转成小写字母 (2) ... */
inline std::string unify_token(std::string_view input) {
    std::string unified_token(input);
    std::for_each(unified_token.begin(), unified_token.end(), [](char& c){
        c = std::tolower(c);
    });
    return unified_token;
}

} // namespace ir::common
