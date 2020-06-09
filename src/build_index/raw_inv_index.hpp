#pragma once

#include <common/common.hpp>
#include <common/doc_inv_index.hpp>

#include <common/preprocess.hpp>

#include <string>

#include <filesystem>
#include <fstream>

namespace ir::build_index {
using Raw = std::unordered_map<std::string, std::vector<common::DocInvIndexElement>>;

inline std::pair<common::DocInvIndex, common::Dictionary> raw_to_fi(const Raw &raw) {
    NOT_IMPLEMENTED;
}

inline std::pair<Raw, size_t> build_raw(std::filesystem::directory_iterator dir) {
    Raw raw;
    size_t N = 1;
    for (auto docId = 0; auto &p : dir) {
        N++;

        std::ifstream fin(p.path().string());

        auto input = common::read_file(fin);
        auto ts = common::tokenize(input);
        for (auto pos = 0; auto &&token : ts) {
            auto u_token = common::unify_token(token);
            if (!raw.contains(u_token)) {
                raw[u_token] = {};
            }
            auto &&tk = raw[u_token];
            auto iter = common::binary_search(tk.begin(), tk.end(), [&](auto &&x) {
                return x.doc_id < docId;
            });
            if (iter == tk.end()) {
                common::DocInvIndexElement x;
                x.doc_id = docId;
                x.positions.push_back(pos);
                tk.push_back(std::move(x));
            } else {
                iter->positions.push_back(pos);
            }
        }
    }
    return {std::move(raw), N};
}

} // namespace ir::build_index
