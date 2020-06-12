#pragma once
#include <unordered_map>
#include <vector>

#include "dictionary.hpp"
#include "doc_inv_index.hpp"
#include "find_in_dict.hpp"
#include "preprocess.hpp"
#include "vec.hpp"

namespace ir::common::vec {
inline Vec vec_of_tokens(const std::vector<std::string_view> &tokens,
                         const common::DocInvIndex &index, const common::Dictionary &dict,
                         size_t N) {
    std::unordered_map<size_t, size_t> ts;
    for (auto x : tokens) {
        auto t = common::unify_token(x);
        auto it = common::find_in_dict(t, dict, index);
        assert(it != index.items.end());

        ts[std::distance(index.items.begin(), it)]++;
    }
    Vec ret;
    double norm = 0;
    for (auto &&[t, tf] : ts) {
        double idf = std::log10(N / index.index.find(index.items[t])->second.size());
        double tfidf = std::log(1 + tf) * idf;
        norm += tfidf * tfidf;
        ret.push_back({t, tfidf});
    }
    for (auto &&[p, s] : ret) {
        s /= std::sqrt(norm);
    }
    return ret;
}
} // namespace ir::common::vec
