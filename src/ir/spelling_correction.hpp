#pragma once

#include <common/dictionary.hpp>
#include <common/kgram.hpp>
#include <string_view>

namespace ir::ir {

inline common::Dictionary::Element spelling_correct(std::string_view input,
                                                    const common::Dictionary &kgram_dict,
                                                    const common::KGramInvIndex &kgram_index) {
    NOT_IMPLEMENTED;
}

} // namespace ir::ir
