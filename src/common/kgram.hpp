#pragma once

#include "common.hpp"
#include "dictionary.hpp"
#include "index.hpp"

#include <string_view>
#include <vector>

namespace ir::common {

using KGramInvIndex = Index<Dictionary::Element, Dictionary::Element>;

inline std::vector<Dictionary::Element> kgram(std::string_view token,
                                              const Dictionary &kgram_dict,
                                              const KGramInvIndex &kgram_index) {
    NOT_IMPLEMENTED;
}
} // namespace ir::common
