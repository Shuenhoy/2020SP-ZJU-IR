#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/dictionary.hpp"
#include "common/doc_inv_index.hpp"
#include "common/find_in_dict.hpp"
#include "common/json.hpp"

using json = nlohmann::json;

namespace ir::ir {

inline void parse_jsonl(json &j, std::ifstream &fin) {
    std::unordered_map<std::string, std::string> syn_dict;
    std::string line;
    while (std::getline(fin, line)) {
        auto j_line = json::parse(line);
        std::string word = j_line["word"].get<std::string>();
        if (syn_dict.find(word) == syn_dict.end()) {
            auto synonyms = j_line["synonyms"].get<std::vector<std::string>>();
            if (synonyms.size() > 0) {
                syn_dict[word] = j_line["synonyms"].get<std::vector<std::string>>()[0];
            }
        }
    }

    for (auto const &[w, s] : syn_dict) {
        j[w] = s;
    }
}

inline std::vector<std::string_view> synonym(const std::vector<std::string_view> &uc_tokens,
                                             const json &j, const common::Dictionary &dict,
                                             const common::DocInvIndex &index) {
    std::vector<std::string_view> ret;
    ret.insert(ret.end(), uc_tokens.begin(), uc_tokens.end());
    for (auto token : uc_tokens) {
        if (j.contains(std::string(token))) {
            auto x = common::find_in_dict(common::unify_token(j[std::string(token)].get<std::string>()), dict, index);
            if (x != index.items.end()) { // 该同义词在词典中存在
                ret.push_back(dict.get(*x));
            }
        }
    }
    return ret;
}

} // namespace ir::ir