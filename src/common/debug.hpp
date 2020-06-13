#pragma once

#include "dictionary.hpp"
#include "doc_inv_index.hpp"
#include "index.hpp"
#include "kgram.hpp"
#include "lead_follow_inv_index.hpp"

#include <fstream>

namespace ir::common::debug {

inline void display(const Dictionary::Element &token, const Dictionary &dict, std::ofstream &fout) {
    fout << dict.get(token);
}

inline void display(const DocInvIndexElement &ele, std::ofstream &fout) {
    fout << "{ ID = " << ele.doc_id;
    for (auto x : ele.positions) {
        fout << ", " << x;
    }
    fout << "}";
}

template <typename T>
inline void display(const Dictionary &dict,
                    const Index<Dictionary::Element, T> &index, std::ofstream &fout) {
    fout << "[Dictionary Begin]\n";
    for (auto x : index.items) {
        display(x, dict, fout);
        fout << "\n";
    }
    fout << "[Dictionary End]\n"
}

template <typename T>
inline void display(const Index<Dictionary::Element, T> &index, const Dictionary &dict, std::ofstream &fout) {
    fout << "[Index Begin]\n";
    for (auto x : index.items) {
        display(x, dict, fout);
        fout << ": [";
        for (auto &&y : index.index[x]) {
            display(y, fout);
            fout << ", ";
        }
        fout << "]\n";
    }
    fout << "[Index End]\n"
}

template <typename T>
inline void display(LeadFollowInvIndex &lead_follow, std::ofstream &fout) {
    fout << "[Index Begin]\n";
    for (auto x : index.items) {
        fout << x << ": [";
        for (auto y : index.index[x]) {
            fout << y << ", ";
        }
        fout << "]\n";
    }
    fout << "[Index End]\n"
}

inline void display(DocumentInfos &docinfos, std::ofstream &fout) {
    fout << "[DocInfos Begin]\n";

    for (auto &&x : docinfos) {
        fout << x.file_name << " " << x.norm << "\n";
    }
    fout << "[End Begin]\n";
}
} // namespace ir::common::debug
