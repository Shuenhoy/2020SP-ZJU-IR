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

    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] != '*') {
            size_t begin = i;
            while (++i < input.size() && input[i] != '*')
                ;
            size_t len = i - begin;
            if (len <= k) {
                auto part_kgrams = common::kgram(input.substr(begin, len), k, kgram_dict, kgram_index);
                kgrams.insert(kgrams.end(), part_kgrams.begin(), part_kgrams.end());
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
