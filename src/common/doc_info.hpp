#pragma once

#include <string>
#include <vector>

#include "serialization.hpp"

namespace ir::common {
struct DocumentInfoElement {
    std::string file_name;
    double norm;
};

using DocumentInfos = std::vector<DocumentInfoElement>;

template <>
struct Serialization<DocumentInfos> {
    static void serialize(std::ofstream &fout, const DocumentInfos &a) {
        NOT_IMPLEMENTED;
    }
    static DocumentInfos deserialize(std::ifstream &fin) {
        NOT_IMPLEMENTED;
    }
};

} // namespace ir::common
