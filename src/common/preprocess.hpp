#pragma once

#include "common.hpp"
#include <cctype>
#include <string>
#include <string_view>
#include <vector>

namespace ir::common {

/* 将字符串变成 token，目前仅仅保留了字母与数字，其他符号全部去除 */
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

inline std::string unify_token(std::string_view input) {
    NOT_IMPLEMENTED;
}

} // namespace ir::common
