#pragma once
#include <fstream>

#include "common.hpp"
#include "serialization.hpp"

namespace ir::common {

// VB code
template <>
struct Serialization<size_t> {
    static void serialize(std::ofstream &fout, size_t &a) {
        NOT_IMPLEMENTED;
    }
    static size_t deserialize(std::ifstream &fin) {
        NOT_IMPLEMENTED;
    }
};
} // namespace ir::common