#pragma once

#include <common/doc_inv_index.hpp>

#include "index_op.hpp"
#include <memory>

namespace ir::ir {

struct BooleanOperator {
    virtual std::vector<size_t> exec() = 0;
};

struct BooleanOperatorUnion : public BooleanOperator {
    std::unique_ptr<BooleanOperator> left, right;
    virtual std::vector<size_t> exec() {
        return IndexOp<size_t>::index_union(left->exec(), right->exec());
    }
};

struct BooleanOperatorIntersection : public BooleanOperator {
    std::unique_ptr<BooleanOperator> left, right;

    virtual std::vector<size_t> exec() {
        return IndexOp<size_t>::index_intersection(left->exec(), right->exec());
    }
};

struct BooleanOperatorDifference : public BooleanOperator {
    std::unique_ptr<BooleanOperator> left, right;

    virtual std::vector<size_t> exec() {
        return IndexOp<size_t>::index_difference(left->exec(), right->exec());
    }
};

struct BooleanOperatorToken : public BooleanOperator {
    virtual std::vector<size_t> exec() {
        NOT_IMPLEMENTED;
    }
};

struct BooleanOperatorWildcard : public BooleanOperator {
    virtual std::vector<size_t> exec() {
        NOT_IMPLEMENTED;
    }
};

struct BooleanOperatorPhrase : public BooleanOperator {
    virtual std::vector<size_t> exec() {
        NOT_IMPLEMENTED;
    }
};

inline const std::vector<size_t> &
remove_position(const common::DocInvIndexElement &ind) {
    return ind.doc_ids;
}

} // namespace ir::ir