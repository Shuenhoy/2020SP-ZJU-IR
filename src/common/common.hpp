#pragma once

#include <cstddef>
#include <functional>

#define NOT_IMPLEMENTED                      \
    static_assert(false, "NOT_IMPLEMENTED"); \
    throw 0

namespace ir::common {

template <class T>
inline void hash_combine(size_t &seed, T const &v) {
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

} // namespace ir::common