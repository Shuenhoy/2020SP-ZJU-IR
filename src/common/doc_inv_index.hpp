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
        ret.doc_id = Serialization<size_t>::deserialize(fin);
        ret.positions = Serialization<std::vector<size_t>>::deserialize(fin);

        return ret;
    }
};

using DocInvIndex = Index<Dictionary::Element, DocInvIndexElement>;

} // namespace ir::common
