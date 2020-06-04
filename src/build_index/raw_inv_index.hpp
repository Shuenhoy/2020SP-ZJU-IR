#pragma once

#include <common/common.hpp>
#include <common/doc_inv_index.hpp>

#include <common/preprocess.hpp>

#include <string>

#include <filesystem>
#include <fstream>

namespace ir::build_index {
using Raw = std::unordered_map<std::string, common::DocInvIndexElement>;

inline std::pair<common::DocInvIndex, common::Dictionary> raw_to_fi(const Raw &raw) {
    NOT_IMPLEMENTED;
}

inline Raw build_raw(std::filesystem::directory_iterator dir) {
    Raw raw;
    for (auto docId = 0; auto &p : dir) {

        std::ifstream fin(p.path().string());

        auto input = common::read_file(fin);
        auto ts = common::tokenize(input);
        for (auto pos = 0; auto &&token : ts) {
            auto u_token = common::unify_token(token);
            if (!raw.contains(u_token)) {
                raw[u_token] = {};
            }
            auto &&tk = raw[u_token];
            auto iter = common::binary_search(tk.doc_ids.begin(), tk.doc_ids.end(), docId);
            auto iter_p = tk.positions.end();
            if (iter == tk.doc_ids.end()) {
                tk.doc_ids.emplace_back(docId);
                tk.positions.emplace_back();
                iter_p = tk.positions.end() - 1;
            }
            iter_p->emplace_back(pos);
        }
    }
    return raw;
}

} // namespace ir::build_index
