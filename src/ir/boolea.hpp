#pragma once

#include <common/doc_inv_index.hpp>
#include <common/find_in_dict.hpp>
#include <common/preprocess.hpp>
#include <stack>

#include "index_op.hpp"
#include "phrase.hpp"
#include "spelling_correction.hpp"
#include "wildcard.hpp"
#include <memory>

namespace ir::ir {

inline bool is_op(const char *op, std::string_view token) {
    return token.compare(op) == 0;
}

inline bool is_phrase_begin(std::string_view token) {
    return token.starts_with("\"");
}

inline bool is_phrase_end(std::string_view token) {
    return token.ends_with("\"");
}

inline bool is_wildcard(std::string_view token) {
    for (char c : token)
        if (c == '*')
            return true;
    return false;
}

inline std::vector<size_t>
remove_position(const std::vector<common::DocInvIndexElement> &ind) {
    std::vector<size_t> ret;
    std::transform(ind.begin(), ind.end(), std::back_inserter(ret), [](auto &&x) {
        return x.doc_id;
    });
    return ret;
}

inline std::vector<size_t>
index_merge(const std::vector<size_t> &origin, const std::vector<size_t> &part, bool INV, bool AND, const std::vector<size_t> &all) {
    switch ((!!INV << 1) + !!AND) {
    case 0:
        return IndexOp<size_t>::index_union(origin, part);
    case 1:
        return IndexOp<size_t>::index_intersection(origin, part);
    case 2:
        return IndexOp<size_t>::index_union(origin, IndexOp<size_t>::index_difference(all, part));
    case 3:
        return IndexOp<size_t>::index_difference(origin, part);
    default:
        throw std::runtime_error("unreachable!");
    }
}

/* 给定布尔表达式 input，返回最终的文档 ID 列表。
   布尔表达式支持 OR、AND、NOT、短语（用双引号扩起来）和通配符（带*号）。
   没有构建逆波兰表达式，不支持自定义优先级（即括号）和短语中的通配符。 */
inline std::vector<size_t> bool_eval(const std::string &input,
                                     const size_t k,
                                     const double threshold,
                                     const std::vector<size_t> all,
                                     const common::Dictionary &dict,
                                     const common::DocInvIndex &index,
                                     const common::Dictionary &kgram_dict,
                                     const common::KGramInvIndex &kgram_index) {

    std::vector<std::string_view> tokens = common::tokenize(input, true);
    std::vector<size_t> ret;

    bool INV = false;
    bool AND = false;
    for (size_t i = 0; i < tokens.size();) {
        /* 遇到与、或、非时改变状态，并跳过 */
        if (is_op("NOT", tokens[i])) {
            i++;
            INV = true;
        } else if (is_op("AND", tokens[i])) {
            i++;
            AND = true;
            if (is_op("NOT", tokens[i])) {
                i++;
                INV = true;
            } else {
                INV = false;
            }
        } else if (is_op("OR", tokens[i])) {
            i++;
            AND = false;
            if (is_op("NOT", tokens[i])) {
                i++;
                INV = true;
            } else {
                INV = false;
            }
        }

        /* 不可能连续遇到两个布尔运算符，因此此时应是单个单词、短语或带有通配符的单个单词 */
        if (is_phrase_begin(tokens[i])) { // 短语
            /* 将前后双引号去除并归一化 */
            std::vector<std::string_view> phrase(1, common::unify_token(tokens[i].substr(1, tokens[i].size() - 1)));
            i++;
            while (!is_phrase_end(tokens[i])) {
                phrase.push_back(common::unify_token(tokens[i]));
                i++;
            }
            phrase.push_back(common::unify_token(tokens[i].substr(0, tokens[i].size() - 1)));
            i++;

            /* 对短语中每个单词对应的文档 ID 取交集（带位置信息） */
            auto token_d = spelling_correct(phrase[0], k, threshold, dict, kgram_dict, kgram_index);
            std::vector<common::DocInvIndexElement> ph_inv_idx = index.index.at(token_d);

            for (size_t i = 1; i < phrase.size(); i++) {
                auto token_d = spelling_correct(phrase[i], k, threshold, dict, kgram_dict, kgram_index);
                auto inv_idx_eles = index.index.at(token_d);
                ph_inv_idx = phrase_merge(ph_inv_idx, inv_idx_eles);
            }

            /* 融合到结果中 */
            ret = index_merge(ret, remove_position(ph_inv_idx), INV, AND, all);
        } else if (is_wildcard(tokens[i])) { // 通配符
            auto token_ds = wildcard(common::unify_token(tokens[i]), k, dict, kgram_dict, kgram_index);
            for (auto token_d : token_ds) {
                ret = index_merge(ret, remove_position(index.index.at(token_d)), INV, AND, all);
            }
            i++;
        } else { // 单个词项
            auto token_d = spelling_correct(common::unify_token(tokens[i]), k, threshold, dict, kgram_dict, kgram_index);
            ret = index_merge(ret, remove_position(index.index.at(token_d)), INV, AND, all);
            i++;
        }
    }

    return ret;
}

} // namespace ir::ir