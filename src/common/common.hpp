#pragma once

#include <cstddef>
#include <fstream>
#include <functional>
#include <string>
#include <type_traits>

#define NOT_IMPLEMENTED                      \
    static_assert(false, "NOT_IMPLEMENTED"); \
    throw 0

namespace ir::common {

template <class T>
inline void hash_combine(size_t &seed, T const &v) {
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

inline std::string read_file(std::ifstream &t) {
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    return str;
}

template <class ForwardIt, class Compare>
requires std::is_invocable_r_v<bool, Compare, typename ForwardIt::value_type>
    ForwardIt binary_search(ForwardIt first, ForwardIt last, Compare comp) {
    ForwardIt it;
    typename std::iterator_traits<ForwardIt>::difference_type count, step;
    count = std::distance(first, last);

    while (count > 0) {
        it = first;
        step = count / 2;
        std::advance(it, step);
        if (comp(*it)) {
            first = ++it;
            count -= step + 1;
        } else
            count = step;
    }
    return first != last && !comp(*first) ? first : last;
}

template <class ForwardIt, class T, class Compare = std::less<>>
requires std::is_invocable_r_v<bool, Compare, T, T>
    ForwardIt binary_search(ForwardIt first, ForwardIt last, const T &value, Compare comp = {}) {
    first = std::lower_bound(first, last, value, comp);
    return first != last && !comp(value, *first) ? first : last;
}

} // namespace ir::common

namespace std {
template <typename T, typename V>
struct hash<std::pair<T, V>> {
    size_t operator()(const std::pair<T, V> &e) const {
        size_t seed = 0;
        ir::common::hash_combine(seed, e.first);
        ir::common::hash_combine(seed, e.second);
        return seed;
    }
};
}