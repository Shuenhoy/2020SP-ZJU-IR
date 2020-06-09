#pragma once
#include <fstream>

#include "common.hpp"
#include "serialization.hpp"

#include <ranges>

namespace ir::common {

// VB code
template <>
struct Serialization<size_t> {
    static void serialize(std::ofstream &fout, const size_t &b) {
        std::vector<uint8_t> bytes;
        size_t a = b;

        for (;;) {
            bytes.push_back(a % 128);
            if (a < 128)
                break;
            a = a / 128;
        }
        bytes[0] += 128;
        for (auto i : std::ranges::views::reverse(bytes)) {
            fout.write(reinterpret_cast<const char *>(&i), 1);
        }
    }
    static size_t deserialize(std::ifstream &fin) {
        uint8_t byte = 0;
        size_t n = 0;
        while (!fin.eof()) {
            fin.read(reinterpret_cast<char *>(&byte), 1);
            if (byte < 128) {
                n = 128 * n + byte;
            } else {
                n = 128 * n + (byte - 128);
                return n;
            }
        }
        throw std::runtime_error("VB broken!");
    }
};

} // namespace ir::common