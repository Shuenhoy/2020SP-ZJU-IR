#pragma once

#include "dictionary.hpp"
#include "doc_inv_index.hpp"
#include "index.hpp"
#include "kgram.hpp"
#include "lead_follow_inv_index.hpp"

#include <fstream>

namespace ir::common::debug {

inline void display(const Dictionary::Element &token, const Dictionary &dict, std::ofstream &fout) {
    fout << dict.get(token) << "(" << token.pos << "," << token.len << ")";
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
    fout << "string length: " << dict.dic.size() << "\n";
    fout << "tokens: " << index.items.size() << "\n";
    for (auto x : index.items) {
        display(x, dict, fout);
        fout << "\n";
    }
    fout << "[Dictionary End]\n";
    fout.flush();
}

inline void display(DocInvIndex &index, const Dictionary &dict, std::ofstream &fout) {
    fout << "[DocIndex Begin]\n";
    for (auto x : index.items) {
        display(x, dict, fout);
        fout << ": [";
        for (auto &&y : index.index.at(x)) {

            display(y, fout);
            fout << ", ";
        }
        fout << "]\n";
    }
    fout << "[DocIndex End]\n";
    fout.flush();
}

inline void display(KGramInvIndex &index, const Dictionary &kdict, const Dictionary &dict, std::ofstream &fout) {
    fout << "[KGramIndex Begin]\n";
    for (auto x : index.items) {
        display(x, kdict, fout);
        fout << ": [";
        for (auto &&y : index.index.at(x)) {
            display(y, dict, fout);

            fout << ", ";
        }
        fout << "]\n";
    }
    fout << "[KGramIndex End]\n";
    fout.flush();
}

inline void display(const LeadFollowInvIndex &lead_follow, std::ofstream &fout) {
    fout << "[Index Begin]\n";
    for (auto x : lead_follow.items) {
        fout << x << ": [";
        for (auto y : lead_follow.index.at(x)) {
            fout << y << ", ";
        }
        fout << "]\n";
    }
    fout << "[Index End]\n";
    fout.flush();
}

inline void display(DocumentInfos &docinfos, std::ofstream &fout) {
    fout << "[DocInfos Begin]\n";

    for (auto &&x : docinfos) {
        fout << x.file_name << " " << x.norm << "\n";
    }
    fout << "[End Begin]\n";
    fout.flush();
}
} // namespace ir::common::debug
