#include "doc_info.hpp"
#include "kgram.hpp"
#include "lead_follow.hpp"
#include "raw.hpp"
#include "raw_inv_index.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "common/debug.hpp"

using namespace ir;
using namespace std::string_literals;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "usage: build_index <input dir> <output dir> <kgram>" << std::endl;
        return -1;
    }
    auto input_dir = std::filesystem::directory_iterator(argv[1]);
    std::string output_dir = argv[2];

    size_t k = std::atol(argv[3]);
    std::cout << "building inv doc index" << std::endl;
    auto [raw, N] = build_index::build_raw(input_dir);
    auto [doc_index, dict] = build_index::raw_to_fi(raw);
    std::cout << "building doc infos" << std::endl;

    auto doc_infos = build_index::build_doc_infos(input_dir, dict, doc_index, N);
    auto [kgram, kgram_dict] = build_index::raw_to_fi(build_index::build_kgram(dict, doc_index, k));
    std::cout << "building lead follow index" << std::endl;

    auto lead_follow = build_index::build_lead_follow(doc_infos, dict, doc_index);
    std::cout << "writing to disk" << std::endl;

    {
        std::ofstream fout(output_dir + "/doc.index", std::ios_base::binary);
        doc_index.serialize(fout);
    }
    {
        std::ofstream fout(output_dir + "/kgram.index", std::ios_base::binary);
        kgram.serialize(fout);
    }
    {
        std::ofstream fout(output_dir + "/lead_follow.index", std::ios_base::binary);
        lead_follow.serialize(fout);
    }
    {
        std::ofstream fout(output_dir + "/doc.dict", std::ios_base::binary);
        common::Serialization<common::Dictionary>::serialize(fout, dict);
    }
    {
        std::ofstream fout(output_dir + "/kgram.dict", std::ios_base::binary);
        common::Serialization<common::Dictionary>::serialize(fout, kgram_dict);
    }
    {
        std::ofstream fout(output_dir + "/doc.infos", std::ios_base::binary);
        common::Serialization<common::DocumentInfos>::serialize(fout, doc_infos);
    }
    std::cout << "done." << std::endl;
#ifdef DEBUG
    std::ofstream fout(output_dir + "/debug.log");

    common::debug::display(doc_infos, fout);
    common::debug::display(kgram_dict, kgram, fout);
    common::debug::display(kgram, kgram_dict, fout);
    common::debug::display(lead_follow, fout);
    common::debug::display(dict, doc_index, fout);
    common::debug::display(doc_index, dict, fout);
#endif

    return 0;
}