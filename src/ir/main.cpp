#include <cassert>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

#include "boolea.hpp"
#include "index_op.hpp"
#include "phrase.hpp"
#include "spelling_correction.hpp"
#include "topk.hpp"
#include "wildcard.hpp"

#include <common/vec_of_tokens.hpp>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

inline void log(std::string s) {
    std::cout << "[LOG] " << s << std::endl;
}

void print_result(std::vector<size_t> docids, ir::common::DocumentInfos doc_infos, double duration) {
    /* TODO: 输出匹配部分摘要 */
    if (docids.size() == 0) {
        std::cout << "No result from " << doc_infos.size() << " documents";
        printf(" in %.4lf seconds.\n", duration);
        return;
    }
    printf(">>> %d results from %d documents:\n", docids.size(), doc_infos.size());
    printf("\n%8s %8s %30s\n", "Rank", "Doc ID", "File path");
    for (size_t i = 0; i < docids.size(); i++) {
        printf("%8d %8d %30s\n", i + 1, docids[i], doc_infos[docids[i]].file_name.c_str());
    }
    printf("\nin %.4lf seconds.\n", duration);
}

int main(int argc, char *argv[]) {
#ifndef NDEBUG
    auto file_logger = spdlog::basic_logger_mt("basic_logger", "test/logs.txt");
    spdlog::set_default_logger(file_logger);
    spdlog::set_level(spdlog::level::debug);

#endif

    if (argc != 4) {
        std::cerr << "Usage: ir <index dir> <kgram> <similarity-threshold>" << std::endl;
        return -1;
    }
    std::string index_dir = argv[1];
    size_t k = std::atol(argv[2]);         // K-Gram 中的 k 值
    double threshold = std::atof(argv[3]); // K-Gram 拼写矫正阈值

    std::string prompt = "\n\n======= SEARCH MODE =======\n"\
                         "[1] Boolean. [2] TopK. [3] Accurate TopK\n"\
                         "[quit] to exit the program> ";

    /* 读取文档倒排索引、文档词典、文档信息，KGram 索引和 KGram 词典，LeadFollow 索引 */
    log("Loading index from disk...");
    std::ifstream fin;

    fin.open(index_dir + "/doc.index", std::ios_base::binary);
    assert(fin.good());
    auto doc_index = ir::common::DocInvIndex::deserialize(fin);
    fin.close();
    log("Documents index OK.");

    fin.open(index_dir + "/doc.dict", std::ios_base::binary);
    assert(fin.good());
    auto doc_dict = ir::common::Serialization<ir::common::Dictionary>::deserialize(fin);
    fin.close();
    log("Documents dictionary OK.");

    fin.open(index_dir + "/doc.infos", std::ios_base::binary);
    assert(fin.good());
    auto doc_infos = ir::common::Serialization<ir::common::DocumentInfos>::deserialize(fin);
    fin.close();
    log("Documents information OK.");

    fin.open(index_dir + "/kgram.index", std::ios_base::binary);
    assert(fin.good());
    auto kgram_index = ir::common::KGramInvIndex::deserialize(fin);
    fin.close();
    log("KGrams index OK.");

    fin.open(index_dir + "/kgram.dict", std::ios_base::binary);
    assert(fin.good());
    auto kgram_dict = ir::common::Serialization<ir::common::Dictionary>::deserialize(fin);
    fin.close();
    log("KGrams dictionary OK.");

    fin.open(index_dir + "/lead_follow.index", std::ios_base::binary);
    assert(fin.good());
    auto leadfollow = ir::common::LeadFollowInvIndex::deserialize(fin);
    fin.close();
    log("LeadFollows index OK.");

    log("Done.");

    /* 文档 ID 全集，用于布尔取反 */
    std::vector<size_t> all(doc_infos.size());
    for (size_t i = 0; i < all.size(); i++) {
        all[i] = i;
    }
    log("Total documents: " + std::to_string(doc_infos.size()));

    for (;;) {
        std::string mode;
        std::string query;
        std::vector<size_t> result;
        std::cout << prompt;
        std::cin >> mode;
        if (mode[0] == 'q') {
            std::cout << "Bye." << std::endl;
            break;
        }

        std::chrono::steady_clock::time_point begin_time;
        if (mode == "1") {
            std::cout << "\nBoolean Syntax: [NOT] <query> [<AND|OR> [NOT] <query>]... \n  query := term | \"some phrases\" | term*with*wildcard\n> ";
            while (query.size() == 0)
                std::getline(std::cin, query);
            begin_time = std::chrono::steady_clock::now();
            result = ir::ir::bool_eval(query, k, threshold, all, doc_dict, doc_index, kgram_dict, kgram_index);
        } else if (mode == "2" || mode == "3") {
            std::cout << "\nTopK Syntax: <K> <query_line>\n> ";
            size_t K;
            std::cin >> K;
            while (query.size() == 0)
                std::getline(std::cin, query);

            begin_time = std::chrono::steady_clock::now();
            auto tokens = ir::common::tokenize(query, false);
            std::vector<std::string_view> uc_tokens;  // unified and corrected tokens
            for (auto &token : tokens) {
                uc_tokens.push_back(
                    doc_dict.get(
                        ir::ir::spelling_correct(
                            ir::common::unify_token(token), k, threshold, doc_dict, kgram_dict, kgram_index
                        )
                    )
                );
            }
            auto query_vec = ir::common::vec::vec_of_tokens(uc_tokens, doc_index, doc_dict, doc_infos.size());
            if (mode == "2")
                result = ir::ir::topk(query_vec, K, leadfollow, doc_index, doc_infos);
            else
                result = ir::ir::acc_topk(query_vec, K, doc_index, doc_infos);
        } else {
            std::cout << "Mode error. Try again." << std::endl;
            continue;
        }
        auto end_time = std::chrono::steady_clock::now();
        double duration = (std::chrono::duration_cast<std::chrono::microseconds>(end_time - begin_time).count()) / 1000000.0;
        print_result(result, doc_infos, duration);
    }

    return 0;
}