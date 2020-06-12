#include <chrono>
#include <cstdio>
#include <iostream>
#include <string>

#include "boolea.hpp"
#include "index_op.hpp"
#include "phrase.hpp"
#include "topk.hpp"
#include "wildcard.hpp"
#include <common/vec_of_tokens.hpp>

inline void log(std::string s) {
    std::cout << "[LOG] " << s << std::endl;
}

void print_result(std::vector<size_t> docids, size_t total_doc_nums, double duration) {
    printf(">>> %d results from %d documents:\n", docids.size(), total_doc_nums);
    printf("\n%8s %15s\n", "Rank", "Document ID");
    for (size_t i = 0; i < docids.size(); i++) {
        printf("%8d %15d\n", i + 1, docids[i]);
    }
    printf("\nin %.2lf seconds.\n", duration);
}

int main() {
    size_t k;         // K-Gram 中的 k 值
    double threshold; // K-Gram 拼写矫正阈值
    std::string prompt = "Select your search mode: 1) boolean. 2) topk.\nInput quit to exit the program.";

    /* 读取文档倒排索引、文档词典、文档信息，KGram 索引和 KGram 词典，LeadFollow 索引 */
    log("Loading index from disk...");
    ir::common::Dictionary dict;
    ir::common::DocInvIndex index;
    ir::common::DocumentInfos doc_infos;
    ir::common::Dictionary kgram_dict;
    ir::common::KGramInvIndex kgram_index;
    ir::common::LeadFollowInvIndex leadfollow;
    log("Done.");

    /* 文档 ID 全集，用于布尔取反 */
    std::vector<size_t> all(doc_infos.size());
    for (size_t i = 0; i < all.size(); i++) {
        all[i] = i;
    }

    for (;;) {
        std::string mode;
        std::string query;
        std::vector<size_t> result;
        std::cout << prompt << std::endl;
        std::cin >> mode;
        if (mode[0] == 'q') {
            std::cout << "Bye." << std::endl;
            break;
        }

        std::chrono::steady_clock::time_point begin_time = std::chrono::steady_clock::now();
        if (mode == "1") {
            std::cout << "Boolean search> ";
            std::getline(std::cin, query);
            result = ir::ir::bool_eval(query, k, threshold, all, dict, index, kgram_dict, kgram_index);
        } else {
            std::cout << "TopK search> ";
            size_t K;
            std::cin >> K;
            std::getline(std::cin, query);
            std::vector<std::string_view> tokens = ir::common::tokenize(query);
            ir::common::vec::Vec query_vec = ir::common::vec::vec_of_tokens(tokens, index, dict, doc_infos);
            result = ir::ir::topk(query_vec, K, leadfollow, index, doc_infos);
        }
        std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
        double duration = (std::chrono::duration_cast<std::chrono::microseconds>(end_time - begin_time).count()) / 1000000.0;
        print_result(result, doc_infos.size(), duration);
    }

    return 0;
}