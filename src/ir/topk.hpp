#pragma once

#include <common/doc_inv_index.hpp>
#include <common/lead_follow_inv_index.hpp>
#include <common/vec.hpp>

#include <vector>

namespace ir::ir {
std::vector<size_t> topk(const common::vec::Vec &vec, size_t K,
                         const common::LeadFollowInvIndex &lead_follow,
                         const common::DocInvIndex *doc_inv) {
    NOT_IMPLEMENTED;
}
} // namespace ir::ir
