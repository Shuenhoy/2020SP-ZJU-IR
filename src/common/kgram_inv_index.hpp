#pragma once

#include "common.hpp"
#include "dictionary.hpp"
#include "index.hpp"
#include <vector>

namespace ir::common {

using KGramInvIndex = Index<Dictionary::Element, Dictionary::Element>;
} // namespace ir::common
