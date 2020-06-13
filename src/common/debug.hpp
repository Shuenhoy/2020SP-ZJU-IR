#pragma once

#include "dictionary.hpp"
#include "doc_inv_index.hpp"
#include "index.hpp"
#include "kgram.hpp"
#include "lead_follow_inv_index.hpp"
#include "vec.hpp"

#include <fstream>

namespace ir::common::debug {

inline void display(const vec::Vec &vec, const Dictionary &dict, const DocInvIndex &index, std::ostream &fout) {
    fout << "(";
    for (auto &&[token, value] : vec) {
        fout << "{";
        fout << dict.get(index.items[token]) << ": " << value;
        fout << "}, ";
    }
    fout << ")";
}

inline void display(const vec::Vec &vec, const DocInvIndex &index, std::ostream &fout) {
    fout << "(";
    for (auto &&[token, value] : vec) {
        fout << "{";
        fout << "(" << token << ", " << index.items[token].pos << ", " << index.items[token].len << ")"
             << ": " << value;
        fout << "}, ";
    }
    fout << ")";
}

inline void display(const Dictionary::Element &token, const Dictionary &dict, std::ostream &fout) {
    fout << dict.get(token) << "(" << token.pos << "," << token.len << ")";
}

inline void display(const DocInvIndexElement &ele, std::ostream &fout) {
    fout << "{ ID = " << ele.doc_id;
    for (auto x : ele.positions) {
        fout << ", " << x;
    }
    fout << "}";
}

template <typename T>
inline void display(const Dictionary &dict,
                    const Index<Dictionary::Element, T> &index, std::ostream &fout) {
    fout << "[Dictionary Begin]\n";
    fout << "string length: " << dict.dic.size() << "\n";
    fout << "tokens: " << index.items.size() << "\n";
    for (auto i = 0; auto x : index.items) {
        fout << i << ", ";
        display(x, dict, fout);
        fout << "\n";
        i++;
    }
    fout << "[Dictionary End]\n";
    fout.flush();
}

inline void display(DocInvIndex &index, const Dictionary &dict, std::ostream &fout) {
    fout << "[DocIndex Begin]\n";
    for (auto i = 0; auto x : index.items) {
        fout << i << ", ";
        display(x, dict, fout);
        fout << ": [";
        for (auto &&y : index.index.at(x)) {

            display(y, fout);
            fout << ", ";
        }
        fout << "]\n";
        i++;
    }
    fout << "[DocIndex End]\n";
    fout.flush();
}

inline void display(KGramInvIndex &index, const Dictionary &kdict, const Dictionary &dict, std::ostream &fout) {
    fout << "[KGramIndex Begin]\n";
    for (auto i = 0; auto x : index.items) {
        fout << i << ", ";

        display(x, kdict, fout);
        fout << ": [";
        for (auto &&y : index.index.at(x)) {
            display(y, dict, fout);

            fout << ", ";
        }
        fout << "]\n";
        i++;
    }
    fout << "[KGramIndex End]\n";
    fout.flush();
}

inline void display(const LeadFollowInvIndex &lead_follow, std::ostream &fout) {
    fout << "[LeadFollow Begin]\n";
    for (auto i = 0; auto x : lead_follow.items) {
        fout << i << ", " << x << "(" << lead_follow.index.at(x).size() << ")"
             << ": [";
        for (auto y : lead_follow.index.at(x)) {
            fout << y << ", ";
        }
        fout << "]\n";
        i++;
    }
    fout << "[LeadFollow End]\n";
    fout.flush();
}

inline void display(DocumentInfos &docinfos, std::ostream &fout) {
    fout << "[DocInfos Begin]\n";

    for (auto i = 0; auto &&x : docinfos) {
        fout << i << ", " << x.file_name << " " << x.norm << "\n";
        i++;
    }
    fout << "[End Begin]\n";
    fout.flush();
}
} // namespace ir::common::debug
