#pragma once

#include "debug.hpp"
#include "doc_inv_index.hpp"

#include <iostream>

#include <spdlog/spdlog.h>

namespace ir::common {
double cos_dist(const vec::Vec &query, size_t doc, const DocInvIndex &index, const DocumentInfos &doc_infos) {
    spdlog::debug("computing distance between {} and doc {}", debug::display(query, index), doc);

    double ans = 0.0;
    double norm = 0.0;
    for (auto &&x : query) {
        auto &&ts = index.index.at(index.items[x.first]);
        auto item = binary_search(ts.begin(), ts.end(), [doc](const DocInvIndexElement &y) {
            return y.doc_id < doc;
        });
        size_t tf = 0;
        if (item != ts.end() && item->doc_id == doc) {
            tf = item->positions.size();
            spdlog::debug("found token {} in doc {}", x.first, item->doc_id);
        }
        auto df = ts.size();
        double idf = std::log10(doc_infos.size() / static_cast<double>(1 + df));
        spdlog::debug("token {}: {}, {} tf={}, df={}, Ndoc={}", x.first, x.second, std::log10(1 + tf) * idf, tf, df, doc_infos.size());
        ans += x.second * std::log10(1 + tf) * idf;
        norm += x.second * x.second;
    }
    spdlog::debug("ans = {}", ans / std::sqrt(norm) / doc_infos[doc].norm);
    return ans / std::sqrt(norm) / doc_infos[doc].norm;
}
} // namespace ir::common
