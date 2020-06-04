#pragma once

#include <fstream>

namespace ir::common {

template <typename T>
struct Serialization {
    static inline void serialize(std::ofstream &fout, T &&a);
    static inline T deserialize(std::ifstream &fin);
};

} // namespace ir::common
