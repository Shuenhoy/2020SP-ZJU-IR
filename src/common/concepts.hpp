#pragma once

#include <concepts>
#include <fstream>
#include <functional>

#include "serialization.hpp"

namespace ir::common {
template <typename T>
concept Hashable = requires(T a) {
    { std::hash<T>{}(a) }
    ->std::convertible_to<std::size_t>;
};
template <typename T>
concept Equal =
    requires(T a, T b) {
    { a == b }
    ->std::convertible_to<bool>;
    { a != b }
    ->std::convertible_to<bool>;
};

template <typename T, typename V>
struct Comparison {
    static inline bool Compare(T &&a, V &&b);
};

template <typename T>
concept Serializable = requires(T a, std::ofstream &fout, std::ifstream &fin) {
    {Serialization<T>::serialize(fout, a)};

    { Serialization<T>::deserialize(fin) }
    ->std::convertible_to<T>;
    // namespace ir::common
};

} // namespace ir::common
