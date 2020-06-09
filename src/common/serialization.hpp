#pragma once

#include <fstream>
#include <string>

namespace ir::common {

template <typename T>
struct Serialization {
    static inline void serialize(std::ofstream &fout, T &&a);
    static inline T deserialize(std::ifstream &fin);
};

template <>
struct Serialization<double> {
    static void serialize(std::ofstream &fout, const double &b) {
        fout.write(reinterpret_cast<const char *>(&b), 8);
    }
    static size_t deserialize(std::ifstream &fin) {
        char bytes[8];
        fin.read(bytes, 8);
        return *reinterpret_cast<double *>(bytes);
    }
};

} // namespace ir::common
