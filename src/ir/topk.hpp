#pragma once

#include <common/cos_dist.hpp>
#include <common/doc_inv_index.hpp>
#include <common/lead_follow_inv_index.hpp>
#include <common/vec.hpp>

#include <queue>

#include <spdlog/spdlog.h>
#include <vector>

namespace ir::ir {

/* 输入查询向量 query，返回最接近的 K 个文档 */
std::vector<size_t> topk(const common::vec::Vec &query, size_t K,
                         const common::LeadFollowInvIndex &lead_follow,
                         const common::DocInvIndex &doc_inv,
                         const common::DocumentInfos &docinfos) {
    std::vector<size_t> ret;
    std::priority_queue<std::pair<double, size_t>> leader_pq;
    spdlog::debug("comparing with leaders");

    for (size_t leader : lead_follow.items) {
        leader_pq.push({common::cos_dist(query, leader, doc_inv, docinfos), leader});
    }

    while (ret.size() < K && !leader_pq.empty()) {
        size_t leader = leader_pq.top().second;
        leader_pq.pop();
        std::priority_queue<std::pair<double, size_t>> follower_pq;
        spdlog::debug("comparing with leader {}'s followers", leader);

        for (size_t follower : lead_follow.index.at(leader)) {
            spdlog::debug("comparing with follower {}", follower);

            follower_pq.push({common::cos_dist(query, follower, doc_inv, docinfos), follower});
        }
        while (ret.size() < K && !follower_pq.empty()) {
            if (follower_pq.top().first > 0.0001)
                ret.push_back(follower_pq.top().second);
            follower_pq.pop();
        }
    }
    return ret;
}

} // namespace ir::ir
