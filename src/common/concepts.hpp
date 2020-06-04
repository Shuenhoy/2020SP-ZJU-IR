#pragma once

#include <concepts>
#include <fstream>
#include <functional>

namespace ir::common {
template <typename T>
concept Hashable = requires(T a) {
    { std::hash<T>{}(a) }
    ->std::convertible_to<std::size_t>;
};

template <typename T>
concept Serializable = requires(T a, std::ofstream &fout, std::ifstream &fin) {
    {a.serialize(fout)};

    { T::deserialize(fin) }
    ->std::same_as<T>;
    // namespace ir::common
};

} // namespace ir::common
