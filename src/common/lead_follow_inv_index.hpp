#pragma once

#include "common.hpp"
#include "dictionary.hpp"
#include "index.hpp"
#include "serialization.hpp"
#include "vb_io.hpp"
#include "vec.hpp"
#include <vector>

namespace ir::common {

using LeadFollowInvIndex = Index<size_t, size_t, size_t>;
} // namespace ir::common
