#pragma once

#include "dictionary.hpp"
#include "doc_inv_index.hpp"
#include "index.hpp"
#include "kgram.hpp"
#include "lead_follow_inv_index.hpp"
#include "vec.hpp"

#include <fstream>

#include <spdlog/fmt/ostr.h>

namespace ir::common::debug {
template <typename U>
struct Display {
    U u;
    Display(U _u) : u(_u) {}
    template <typename OStream>
    friend OStream &operator<<(OStream &os, const Display<U> &c) {
        c.u(os);
        return os;
    }
};

inline auto display(const vec::Vec &vec, const Dictionary &dict, const DocInvIndex &index) {
    return Display([&](auto &&fout) {
        fout << "(";
        for (auto &&[token, value] : vec) {
            fout << "{";
            fout << dict.get(index.items[token]) << ": " << value;
            fout << "}, ";
        }
        fout << ")";
    });
}

inline auto display(const vec::Vec &vec, const DocInvIndex &index) {
    return Display([&](auto &&fout) {
        fout << "(";
        for (auto &&[token, value] : vec) {
            fout << "{";
            fout << "(" << token << ", " << index.items[token].pos << ", " << index.items[token].len << ")"
                 << ": " << value;
            fout << "}, ";
        }
        fout << ")";
    });
}

inline auto display(const Dictionary::Element &token, const Dictionary &dict) {
    return Display([&](auto &&fout) {
        fout << dict.get(token) << "(" << token.pos << "," << token.len << ")";
    });
}

inline auto display(const DocInvIndexElement &ele) {
    return Display([&](auto &&fout) {
        fout << "{ ID = " << ele.doc_id;
        for (auto x : ele.positions) {
            fout << ", " << x;
        }
        fout << "}";
    });
}

template <typename T>
inline auto display(const Dictionary &dict,
                    const Index<Dictionary::Element, T> &index) {
    return Display([&](auto &&fout) {
        fout << "[Dictionary Begin]\n";
        fout << "string length: " << dict.dic.size() << "\n";
        fout << "tokens: " << index.items.size() << "\n";
        for (auto i = 0; auto x : index.items) {
            fout << i << ", ";
            fout << display(x, dict);
            fout << "\n";
            i++;
        }
        fout << "[Dictionary End]\n";
        fout.flush();
    });
}

inline auto display(DocInvIndex &index, const Dictionary &dict) {
    return Display([&](auto &&fout) {
        fout << "[DocIndex Begin]\n";
        for (auto i = 0; auto x : index.items) {
            fout << i << ", ";
            fout << display(x, dict);
            fout << ": [";
            for (auto &&y : index.index.at(x)) {

                fout << display(y);
                fout << ", ";
            }
            fout << "]\n";
            i++;
        }
        fout << "[DocIndex End]\n";
        fout.flush();
    });
}

inline auto display(KGramInvIndex &index, const Dictionary &kdict, const Dictionary &dict) {
    return Display([&](auto &&fout) {
        fout << "[KGramIndex Begin]\n";
        for (auto i = 0; auto x : index.items) {
            fout << i << ", ";

            fout << display(x, kdict);
            fout << ": [";
            for (auto &&y : index.index.at(x)) {
                fout << display(y, dict);

                fout << ", ";
            }
            fout << "]\n";
            i++;
        }
        fout << "[KGramIndex End]\n";
        fout.flush();
    });
}

inline auto display(const LeadFollowInvIndex &lead_follow) {
    return Display([&](auto &&fout) {
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
    });
}

inline auto display(DocumentInfos &docinfos) {
    return Display([&](auto &&fout) {
        fout << "[DocInfos Begin]\n";

        for (auto i = 0; auto &&x : docinfos) {
            fout << i << ", " << x.file_name << " " << x.norm << "\n";
            i++;
        }
        fout << "[End Begin]\n";
        fout.flush();
    });
}
} // namespace ir::common::debug
