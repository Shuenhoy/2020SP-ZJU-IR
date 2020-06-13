#pragma once

#include <common/dictionary.hpp>
#include <common/kgram.hpp>
#include <common/wildcard_match.hpp>

#include <regex>

#include <string_view>
#include <vector>

namespace ir::ir {

/* 基于 KGram 的通配符查询，返回符合通配符的所有词项（词典项）。 */
inline std::vector<common::Dictionary::Element> wildcard(std::string_view input,
                                                         size_t k,
                                                         const common::Dictionary &dict,
                                                         const common::Dictionary &kgram_dict,
                                                         const common::KGramInvIndex &kgram_index) {
    std::vector<common::Dictionary::Element> kgrams;
    std::vector<common::Dictionary::Element> ret;

    /* 以 * 为分隔符把原字符串分开成多个部分 */
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] != '*') {
            size_t begin = i;
            while (++i < input.size() && input[i] != '*')
                ;
            size_t len = i - begin;
            if (len >= k) {  // 长度大于等于 k 的部分可以获得 KGram
                auto part_kgrams = common::kgram(input.substr(begin, len), k, kgram_dict, kgram_index);
                kgrams.insert(kgrams.end(), part_kgrams.begin(), part_kgrams.end());
            } else {  // 长度小于 k 只能和每个 KGram 都尝试匹配一次，符合的则入选
                std::string wild = "*" + std::string(input.substr(begin, len)) + "*";
                for (auto kgram : kgram_index.items) {
                    if (common::wildcard_match(kgram_dict.get(kgram), wild)) {
                        kgrams.push_back(kgram);
                    }
                }
            }
        }
    }

    for (auto kgram : kgrams) {
        std::vector<common::Dictionary::Element> token_ds = kgram_index.index.at(kgram);
        for (auto token_d : token_ds) {
            if (common::wildcard_match(dict.get(token_d), input)) {
                ret.push_back(token_d);
            }
        }
    }

    return ret;
}
} // namespace ir::ir
