#pragma once

#include <common/doc_inv_index.hpp>
#include <common/lead_follow_inv_index.hpp>
#include <common/vec.hpp>
#include <common/preprocess.hpp>

#include <algorithm>
#include <random>

namespace ir::build_index {
inline common::vec::Vec vec_of_document(const std::vector<std::string_view> &tokens,
                                        double norm) {
    NOT_IMPLEMENTED;
}

inline common::LeadFollowInvIndex build_lead_follow(std::string dir, const common::DocumentInfos &docinfos,
                                                    const common::DocInvIndex &doc_index) {
    common::LeadFollowInvIndex index;
    std::random_device r;
    std::default_random_engine gen(r());
    std::uniform_int_distribution<size_t> uniform_dist(0, std::sqrt(docinfos.size()) - 1);
    std::vector<common::vec::Vec> vecs;
    for (auto i = 0; i < docinfos.size(); i += std::sqrt(docinfos.size())) {
        auto id = uniform_dist(gen) + i;
        auto ret = index.index.insert({id, std::vector<size_t>()});
        index.items.push_back(ret.first);

        std::ifstream fin(dir + docinfos[id].file_name);
        auto input = common::read_file(fin);
        auto ts = common::tokenize(input);
        vecs.push_back(vec_of_document(ts, docinfos[id].norm));
    }
    for (auto i = 0; i < docinfos.size(); i++) {
        std::vector<std::pair<double, size_t>> dis;
        for (auto j = 0; j < index.items.size(); j++) {
            dis.push_back({common::cos_dist(vecs[j], i, doc_index, docinfos), j});
        }
        std::sort(dis.begin(), dis.end());
        for (auto j = 0; j < 3; j++) {
            index.index[dis[j].second].push_back(i);
        }
    }
    return index;
}

} // namespace ir::build_index
