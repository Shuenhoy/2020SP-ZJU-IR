#pragma once

#include <common/dictionary.hpp>
#include <common/index.hpp>
#include "index_op.hpp"
#include <common/kgram.hpp>
#include <string_view>
#include <unordered_set>

namespace ir::ir {

inline double kgram_similarity(std::vector<common::Dictionary::Element>& a, std::vector<common::Dictionary::Element>& b) {
    double intersec_size = IndexOp<common::Dictionary::Element>::index_intersection(a, b).size();
    double union_size = IndexOp<common::Dictionary::Element>::index_union(a, b).size();
    return intersec_size / union_size;
}

/* 基于 K-Gram 重合度的拼写矫正 */
inline common::Dictionary::Element spelling_correct(std::string_view input,
                                                    size_t k,
                                                    double threshold,
                                                    const common::Dictionary &dict,
                                                    const common::Dictionary &kgram_dict,
                                                    const common::KGramInvIndex &kgram_index) {

    std::unordered_set<common::Dictionary::Element> unique_indices;

    auto kgrams = common::kgram(input, k, kgram_dict, kgram_index);
    for (auto kgram : kgrams) {
        auto token_indices = kgram_index.index.at(kgram);
        for (auto token_index : token_indices) {
            unique_indices.insert(token_index);
        }
    }

    double max_similarity = 0;
    common::Dictionary::Element most_similar;
    for (auto index : unique_indices) {
        auto another_kgrams = common::kgram(dict.get(index), k, kgram_dict, kgram_index);
        double similarity = kgram_similarity(kgrams, another_kgrams);
        if (similarity >= threshold) {
            return index;
        }
        if (similarity > max_similarity) {
            most_similar = index;
        }
    }

    /* 没有相似度超过阈值的词项，则输出相似度最大的 */
    return most_similar;
}

} // namespace ir::ir
