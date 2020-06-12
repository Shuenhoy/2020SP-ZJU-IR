#pragma once

#include <filesystem>
#include <fstream>

#include <common/common.hpp>
#include <common/dictionary.hpp>
#include <common/doc_info.hpp>
#include <common/doc_inv_index.hpp>
#include <common/preprocess.hpp>
#include <common/vec_of_tokens.hpp>

namespace ir::build_index {
inline common::DocumentInfos build_doc_infos(std::filesystem::directory_iterator dir,
                                             const common::Dictionary &dict,
                                             const common::DocInvIndex &doc_index,
                                             size_t N) {
    common::DocumentInfos ret;
    for (auto docId = 0; auto &p : dir) {
        std::ifstream fin(p.path().c_str());
        auto input = common::read_file(fin);
        auto ts = common::tokenize(input);
        double norm = 0;

        auto xs = common::vec::vec_of_tokens(ts, doc_index, dict, N);
        for (auto &&x : xs) {
            norm += x.second * x.second;
        }
        common::DocumentInfoElement ele;
        ele.file_name = p.path().c_str();
        ele.norm = std::sqrt(norm);
        ret.push_back(ele);
    }
    return ret;
}
} // namespace ir::build_index
