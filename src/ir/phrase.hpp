#pragma once

#include <common/common.hpp>
#include <common/doc_inv_index.hpp>
#include <vector>

namespace ir::ir {
inline std::vector<common::DocInvIndexElement>
phrase_merge(const std::vector<common::DocInvIndexElement> &a,
             const std::vector<common::DocInvIndexElement> &b) {
    std::vector<common::DocInvIndexElement> ret;
    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        if (a[i].doc_id < b[j].doc_id)
            i++;
        else if (a[i].doc_id > b[j].doc_id)
            j++;
        else {
            common::DocInvIndexElement ele;
            ele.doc_id = a[i].doc_id;
            for (size_t pos_b : b[j].positions) {
                for (size_t k = 0; a[i].positions[k] < pos_b; k++) {
                    if (a[i].positions[k] == pos_b - 1) {
                        ele.positions.push_back(pos_b);
                    }
                }
            }

            if (ele.positions.size() > 0)
                ret.push_back(ele);
            
            i++;
            j++;
        }
    }
    return ret;
}

} // namespace ir::ir
