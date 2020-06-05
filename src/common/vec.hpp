#pragma once

#include <cassert>
#include <vector>

namespace ir::common::vec {
using Vec = std::vector<std::pair<size_t, double>>;
inline double dot(const Vec &a, const Vec &b) {
    assert(a.size() == b.size());
    double ans = 0;
    for (auto i = 0; i < a.size(); i++) {
        assert(a[i].first == b[i].first);
        ans += a[i].second * b[i].second;
    }
    return ans;
}

} // namespace ir::common::vec
