#pragma once

#include "common.hpp"
#include <cctype>
#include <string>
#include <vector>

namespace ir::common {

/* 将字符串变成词项，目前仅仅保留了字母与数字，其他符号全部去除 */
inline std::vector<std::string_view> tokenize(std::string_view input) {
    std::vector<std::string_view> tokens;
    for (std::size_t i = 0; i < input.size(); i++) {
        if (std::isalnum(input[i])) {
            std::size_t begin = i;
            while (std::isalnum(input[++i]))
                ;
            std::size_t length = i - begin;
            std::string_view token(input.data() + begin, length);
            tokens.push_back(token);
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
