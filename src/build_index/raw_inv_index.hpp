#pragma once

#include "raw.hpp"

#include <common/common.hpp>
#include <common/doc_inv_index.hpp>

#include <common/preprocess.hpp>

#include <string>

#include <filesystem>
#include <fstream>
#include <iostream>

namespace ir::build_index {

inline std::pair<Raw<common::DocInvIndexElement>, size_t> build_raw(std::filesystem::directory_iterator dir) {
    Raw<common::DocInvIndexElement> raw;
    size_t N = 1;
    for (auto docId = 0; auto &p : dir) {
        N++;

        std::ifstream fin(p.path().string());

        auto input = common::read_file(fin);
        auto ts = common::tokenize(input, false);
        for (auto pos = 0; auto &&token : ts) {
            auto u_token = common::unify_token(token);
            if (!raw.contains(u_token)) {
                raw[u_token] = {};
            }
            auto &&tk = raw[u_token];
            auto iter = common::binary_search(tk.begin(), tk.end(), [&](auto &&x) {
                return x.doc_id < docId;
            });
            if (iter == tk.end() || iter->doc_id != docId) {
                common::DocInvIndexElement x;
                x.doc_id = docId;
                x.positions.push_back(pos);
                tk.push_back(std::move(x));
            } else {
                iter->positions.push_back(pos);
            }
            pos++;
        }
        docId++;
    }
    return {std::move(raw), N};
}

} // namespace ir::build_index
