#pragma once

#include <string>
#include <vector>

#include "serialization.hpp"

namespace ir::common {
struct DocumentInfoElement {
    std::string file_name;
    double norm;
};

using DocumentInfo = std::vector<DocumentInfoElement>;

template <>
struct Serialization<DocumentInfo> {
    static void serialize(std::ofstream &fout, const DocumentInfo &a) {
        NOT_IMPLEMENTED;
    }
    static DocumentInfo deserialize(std::ifstream &fin) {
        NOT_IMPLEMENTED;
    }
};

} // namespace ir::common
