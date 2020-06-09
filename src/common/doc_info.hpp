#pragma once

#include <string>
#include <vector>

#include "common_serialization.hpp"
#include "serialization.hpp"
#include "vb_io.hpp"

namespace ir::common {
struct DocumentInfoElement {
    std::string file_name;
    double norm;
};

using DocumentInfos = std::vector<DocumentInfoElement>;

template <>
struct Serialization<DocumentInfos> {
    static void serialize(std::ofstream &fout, const DocumentInfos &a) {
        Serialization<size_t>::serialize(fout, a.size());
        for (auto &&x : a) {
            Serialization<std::string>::serialize(fout, x.file_name);
            Serialization<double>::serialize(fout, x.norm);
        }
    }
    static DocumentInfos deserialize(std::ifstream &fin) {
        DocumentInfos infos(Serialization<size_t>::deserialize(fin));
        for (auto i = 0; i < infos.size(); i++) {
            infos[i].file_name = Serialization<std::string>::deserialize(fin);
            infos[i].norm = Serialization<double>::deserialize(fin);
        }
    }
};

} // namespace ir::common
