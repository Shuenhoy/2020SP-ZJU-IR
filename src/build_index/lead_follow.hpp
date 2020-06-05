#pragma once

#include <common/lead_follow_inv_index.hpp>
#include <common/vec.hpp>

namespace ir::build_index {
inline common::vec::Vec vec_of_document(const std::vector<std::string> &tokens,
                                        const common::DocumentInfos &docinfos) {
    NOT_IMPLEMENTED;
}

inline common::LeadFollowInvIndex build_lead_follow(std::filesystem::directory_iterator dir, const common::DocumentInfos &docinfos) {
    NOT_IMPLEMENTED;
}

} // namespace ir::build_index
