#pragma once

#include "debug.hpp"
#include "doc_inv_index.hpp"
#include <iostream>

namespace ir::common {
double cos_dist(const vec::Vec &query, size_t doc, const DocInvIndex &index, const DocumentInfos &doc_infos) {
    std::cerr << std::endl;

    double ans = 0.0;
    double norm = 0.0;
    for (auto &&x : query) {
        auto &&ts = index.index.at(index.items[x.first]);
        auto item = binary_search(ts.begin(), ts.end(), [doc](const DocInvIndexElement &y) {
            return y.doc_id < doc;
        });
        size_t tf = 0;
        if (item != ts.end()) {
            tf = item->positions.size();
        }
        auto df = ts.size();
        double idf = std::log10(doc_infos.size() / static_cast<double>(1 + df));
        ans += x.second * std::log10(1 + tf) * idf;
        norm += x.second * x.second;
    }
    return ans / std::sqrt(norm) / doc_infos[doc].norm;
}
} // namespace ir::common
