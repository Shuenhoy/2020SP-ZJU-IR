#pragma once

#include "common.hpp"
#include "dictionary.hpp"
#include "index.hpp"
#include "serialization.hpp"

#include <vector>

namespace ir::common {
struct DocInvIndexElement {
    size_t tf;
    std::vector<size_t> doc_ids;
    std::vector<std::vector<size_t>> positions;
};

template <>
struct Serialization<DocInvIndexElement> {
    static void serialize(std::ofstream &fout, const DocInvIndexElement &a) {
        NOT_IMPLEMENTED;
    }
    static DocInvIndexElement deserialize(std::ifstream &fin) {
        NOT_IMPLEMENTED;
    }
};

using DocInvIndex = Index<Dictionary::Element, DocInvIndexElement>;
} // namespace ir::common
