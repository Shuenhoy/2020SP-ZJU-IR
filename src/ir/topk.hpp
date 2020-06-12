#pragma once

#include <common/doc_inv_index.hpp>
#include <common/lead_follow_inv_index.hpp>
#include <common/vec.hpp>
#include <queue>

#include <vector>

namespace ir::ir {

/* 输入查询向量 query，返回最接近的 K 个文档 */
std::vector<size_t> topk(const common::vec::Vec &query, size_t K,
                         const common::LeadFollowInvIndex &lead_follow,
                         const common::DocInvIndex &doc_inv,
                         const common::DocumentInfos &docinfos) {
    std::vector<size_t> ret;
    std::priority_queue<std::pair<double, size_t>> leader_pq;
    for (size_t leader : lead_follow.items) {
        leader_pq.push({common::cos_dist(query, leader, doc_inv, docinfos), leader});
    }
    while (ret.size() < K && !leader_pq.empty()) {
        size_t leader = leader_pq.top().second;
        leader_pq.pop();
        std::priority_queue<std::pair<double, size_t>> follower_pq;
        for (size_t follower : lead_follow.index.at(leader)) {
            follower_pq.push({common::cos_dist(query, follower, doc_inv, docinfos), follower});
        }
        while (ret.size() < K && !follower_pq.empty()) {
            ret.push_back(follower_pq.top().second);
            follower_pq.pop();
        }
    }
    return ret;
}

} // namespace ir::ir
