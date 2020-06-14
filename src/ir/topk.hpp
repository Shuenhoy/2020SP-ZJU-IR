#pragma once

#include <common/cos_dist.hpp>
#include <common/doc_inv_index.hpp>
#include <common/lead_follow_inv_index.hpp>
#include <common/vec.hpp>

#include <queue>
#include <unordered_set>

#include <spdlog/spdlog.h>
#include <vector>

namespace ir::ir {

/* 输入查询向量 query，返回最接近的 K 个文档 */
std::vector<size_t> topk(const common::vec::Vec &query, size_t K,
                         const common::LeadFollowInvIndex &lead_follow,
                         const common::DocInvIndex &doc_inv,
                         const common::DocumentInfos &doc_infos) {
    std::priority_queue<std::pair<double, size_t>> leader_pq, docs;
    spdlog::debug("comparing with leaders");

    for (size_t leader : lead_follow.items) {
        leader_pq.push({common::cos_dist(query, leader, doc_inv, doc_infos), leader});
    }

    while (docs.size() < K && !leader_pq.empty()) {
        size_t leader = leader_pq.top().second;
        leader_pq.pop();
        std::priority_queue<std::pair<double, size_t>> follower_pq;
        spdlog::debug("comparing with leader {}'s followers", leader);

        for (size_t follower : lead_follow.index.at(leader)) {
            spdlog::debug("comparing with follower {}", follower);

            follower_pq.push({common::cos_dist(query, follower, doc_inv, doc_infos), follower});
        }
        while (docs.size() < K && !follower_pq.empty()) {
            if (follower_pq.top().first > 0.0001)
                docs.push(follower_pq.top());
            follower_pq.pop();
        }
    }

    std::vector<size_t> ret;
    while (!docs.empty()) {
        if (ret.size() == 0 || ret.back() != docs.top().second)
            ret.push_back(docs.top().second);
        docs.pop();
    }
    return ret;
}

std::vector<size_t> acc_topk(const common::vec::Vec &query, size_t K,
                             const common::DocInvIndex &doc_inv,
                             const common::DocumentInfos &doc_infos) {
    std::vector<size_t> ret;
    std::priority_queue<std::pair<double, size_t>,
                        std::vector<std::pair<double, size_t>>>
        pq;
    for (size_t doc_id = 0; doc_id < doc_infos.size(); doc_id++) {
        pq.push({common::cos_dist(query, doc_id, doc_inv, doc_infos), doc_id});
    }
    for (size_t i = 0; i < K; i++) {
        if (pq.top().first < 0.0001)
            break;
        ret.push_back(pq.top().second);
        pq.pop();
    }
    return ret;
}

} // namespace ir::ir
