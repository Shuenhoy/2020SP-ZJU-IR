#pragma once

#include "concepts.hpp"
#include "serialization.hpp"
#include "vb_io.hpp"

#include <fstream>
#include <string>

namespace ir::common {
template <>
struct Serialization<std::string> {
    static void serialize(std::ofstream &fout, const std::string &a) {
        Serialization<size_t>::serialize(fout, a.size());
        fout.write(a.data(), a.size());
    }
    static std::string deserialize(std::ifstream &fin) {
        std::string ret;
        size_t n = Serialization<size_t>::deserialize(fin);
        ret.resize(n);
        fin.read(ret.data(), n);
        return ret;
    }
};

template <typename T>
requires Serializable<T> struct Serialization<std::vector<T>> {
    static void serialize(std::ofstream &fout, const std::vector<T> &b) {
        Serialization<size_t>::serialize(fout, b.size());
        for (auto &&x : b) {
            Serialization<T>::serialize(fout, x);
        }
    }
    static std::vector<T> deserialize(std::ifstream &fin) {
        std::vector<T> ret(Serialization<size_t>::deserialize(fin));
        for (auto &&x : ret) {
            x = Serialization<T>::deserialize(fin);
        }
        return ret;
    }
};
} // namespace ir::common
