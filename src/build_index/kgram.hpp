#pragma once

#include <common/dictionary.hpp>
#include <common/doc_inv_index.hpp>
#include <common/kgram.hpp>

#include "raw.hpp"

namespace ir::build_index {
inline Raw<ir::common::Dictionary::Element> build_kgram(const common::Dictionary &dic, const common::DocInvIndex &index, size_t k) {
    Raw<ir::common::Dictionary::Element> raw;
    for (auto &&x : index.items) {
        auto token = dic.get(x);
        for (auto i = 0; i < token.size() - k; i++) {
            std::string kg{token.substr(i, k)};
            if (!raw.contains(kg)) {
                raw[kg] = {};
            }
            raw[kg].push_back(x);
        }
    }
    return raw;
}
} // namespace ir::build_index
