#pragma once

#include "common.hpp"
#include "dictionary.hpp"
#include "index.hpp"
#include "serialization.hpp"
#include "vb_io.hpp"
#include "vec.hpp"

#include <vector>

namespace ir::common {
struct DocInvIndexElement {
    std::vector<size_t> doc_ids;
    std::vector<std::vector<size_t>> positions;
    size_t df;
    DocInvIndexElement() : df(0) {}
    DocInvIndexElement(DocInvIndexElement &&rhs) = default;
    DocInvIndexElement(const DocInvIndexElement &rhs) = default;
    DocInvIndexElement &operator=(const DocInvIndexElement &rhs) = default;

    double cos_dist(const vec::Vec &query, size_t doc) const {
        NOT_IMPLEMENTED;
    }
};

template <>
struct Serialization<DocInvIndexElement> {
    static void serialize(std::ofstream &fout, const DocInvIndexElement &a) {
        Serialization<size_t>::serialize(fout, a.df);
        Serialization<std::vector<size_t>>::serialize(fout, a.doc_ids);
        Serialization<std::vector<std::vector<size_t>>>::serialize(fout, a.positions);
    }
    static DocInvIndexElement deserialize(std::ifstream &fin) {
        DocInvIndexElement ret;

        ret.df = Serialization<size_t>::deserialize(fin);
        ret.doc_ids = Serialization<std::vector<size_t>>::deserialize(fin);
        ret.positions = Serialization<std::vector<std::vector<size_t>>>::deserialize(fin);

        return ret;
    }
};

using DocInvIndex = Index<Dictionary::Element, DocInvIndexElement>;

} // namespace ir::common
