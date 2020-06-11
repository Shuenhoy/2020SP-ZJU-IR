#pragma once

#include "common.hpp"
#include "dictionary.hpp"
#include "doc_info.hpp"
#include "index.hpp"
#include "serialization.hpp"
#include "vb_io.hpp"
#include "vec.hpp"

#include <cmath>
#include <vector>

namespace ir::common {
struct DocInvIndexElement {
    size_t doc_id;
    std::vector<size_t> positions;
    DocInvIndexElement() {}
    DocInvIndexElement(DocInvIndexElement &&rhs) = default;
    DocInvIndexElement(const DocInvIndexElement &rhs) = default;
    DocInvIndexElement &operator=(const DocInvIndexElement &rhs) = default;
};

template <>
struct Serialization<DocInvIndexElement> {
    static void serialize(std::ofstream &fout, const DocInvIndexElement &a) {
        Serialization<size_t>::serialize(fout, a.doc_id);
        Serialization<std::vector<size_t>>::serialize(fout, a.positions);
    }
    static DocInvIndexElement deserialize(std::ifstream &fin) {
        DocInvIndexElement ret;
        ret.positions = Serialization<std::vector<size_t>>::deserialize(fin);

        return ret;
    }
};

using DocInvIndex = Index<Dictionary::Element, DocInvIndexElement>;

double cos_dist(const vec::Vec &query, size_t doc, const DocInvIndex &index, const DocumentInfos &doc_infos) {
    double ans = 0.0;
    double norm = 0.0;
    for (auto &&x : query) {
        auto &&ts = index.index.find(index.items[x.first])->second;
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
