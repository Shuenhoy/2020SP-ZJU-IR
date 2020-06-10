#pragma once

#include <common/doc_inv_index.hpp>
#include <common/find_in_dict.hpp>
#include <common/preprocess.hpp>
#include <stack>

#include "index_op.hpp"
#include "spelling_correction.hpp"
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

/* 给定布尔表达式 input，返回最终的文档 ID 列表。
   布尔表达式支持 OR、AND、NOT、短语（用双引号扩起来）和通配符（带*号）。
   不打算支持自定义优先级（即括号），短语中也不能有通配符。 */
inline std::vector<size_t> bool_eval(const std::string &input,
                                     const size_t k,
                                     const double threshold,
                                     const common::Dictionary &dict,
                                     const common::DocInvIndex &index,
                                     const common::Dictionary &kgram_dict,
                                     const common::KGramInvIndex &kgram_index) {

    std::vector<std::string_view> tokens = common::tokenize(input);
    std::stack<std::vector<size_t>> OR_stack;

    bool merge = false;
    for (size_t i = 0; i < tokens.size();) {
        bool invert = false;
        std::vector<size_t> docids;

        /* 遇到与、或、非时改变状态，并跳过此 token */
        if (is_op("NOT", tokens[i])) {
            invert = true;
            i++;
        } else if (is_op("AND", tokens[i])) {
            merge = true;
            i++;
        } else if (is_op("OR", tokens[i])) {
            merge = false;
            i++;
        }

        if (is_phrase_begin(tokens[i])) { // 短语
            std::vector<std::string_view> phrase(1, tokens[i].substr(1, tokens[i].size() - 1));
            i++;
            while (!is_phrase_end(tokens[i])) {
                phrase.push_back(tokens[i++]);
            }
            phrase.push_back(tokens[i].substr(0, tokens[i++].size() - 1));
            NOT_IMPLEMENTED;
        } else if (is_wildcard(tokens[i])) { // 通配符
            NOT_IMPLEMENTED;
        } else {                             // 单个词项
            auto idx = spelling_correct(tokens[i], k, threshold, dict, kgram_dict, kgram_index);
            OR_stack.push(ir::remove_position(index.index.at(idx)));
        }
    }

    /* 将栈中的文档 ID 取并形成最终文档 ID 集合 */
    std::vector<size_t> doc_ids;
    while (!OR_stack.empty()) {
        std::vector<size_t> ids = OR_stack.top();
        OR_stack.pop();
        std::set_union(doc_ids.begin(), doc_ids.end(), ids.begin(), ids.end(), std::back_inserter(doc_ids));
    }

    return doc_ids;
}

} // namespace ir::ir