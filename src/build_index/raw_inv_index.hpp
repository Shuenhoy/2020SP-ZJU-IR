#pragma once

#include <common/common.hpp>
#include <common/doc_inv_index.hpp>
#include <string>

namespace ir::build_index {
using Raw = std::unordered_map<std::string, std::vector<common::DocInvIndexElement>>;

inline std::pair<common::DocInvIndex, common::Dictionary> raw_to_fi(const Raw &raw) {
    NOT_IMPLEMENTED;
}

} // namespace ir::build_index
