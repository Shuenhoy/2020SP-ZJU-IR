#pragma once

#include <common/dictionary.hpp>
#include <common/kgram.hpp>

#include <string_view>
#include <vector>

namespace ir::ir {
inline std::vector<common::Dictionary::Element> wildcard(std::string_view input,
                                                         const common::Dictionary &dict,
                                                         const common::Dictionary &kgram_dict,
                                                         const common::KGramInvIndex &kgram_index) {
    NOT_IMPLEMENTED;
}
} // namespace ir::ir
