#pragma once

#include "common.hpp"
#include "dictionary.hpp"
#include "index.hpp"

#include <string_view>
#include <vector>

namespace ir::common {

using KGramInvIndex = Index<Dictionary::Element, std::string_view, Dictionary::Element>;

inline std::vector<Dictionary::Element> kgram(std::string_view token, const Dictionary &kgram_dic) {
    NOT_IMPLEMENTED;
}
} // namespace ir::common
