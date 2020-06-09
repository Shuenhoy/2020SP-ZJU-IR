#pragma once

#include <common/doc_inv_index.hpp>

#include "index_op.hpp"
#include <memory>

namespace ir::ir {

struct BooleanOperator {
    virtual std::vector<size_t> exec() = 0;
};

struct BooleanOperatorUnion : public BooleanOperator {
    BooleanOperatorUnion(BooleanOperatorUnion &&rhs) = default;
    BooleanOperatorUnion &operator=(BooleanOperatorUnion &&rhs) = default;

    std::unique_ptr<BooleanOperator> left, right;
    virtual std::vector<size_t> exec() {
        return IndexOp<size_t>::index_union(left->exec(), right->exec());
    }
};

struct BooleanOperatorIntersection : public BooleanOperator {
    BooleanOperatorIntersection(BooleanOperatorIntersection &&rhs) = default;
    BooleanOperatorIntersection &operator=(BooleanOperatorIntersection &&rhs) = default;

    std::unique_ptr<BooleanOperator> left, right;

    virtual std::vector<size_t> exec() {
        return IndexOp<size_t>::index_intersection(left->exec(), right->exec());
    }
};

struct BooleanOperatorDifference : public BooleanOperator {

    BooleanOperatorDifference(BooleanOperatorDifference &&rhs) = default;
    BooleanOperatorDifference &operator=(BooleanOperatorDifference &&rhs) = default;

    std::unique_ptr<BooleanOperator> left, right;

    virtual std::vector<size_t> exec() {
        return IndexOp<size_t>::index_difference(left->exec(), right->exec());
    }
};

struct BooleanOperatorToken : public BooleanOperator {

    BooleanOperatorToken(BooleanOperatorToken &&rhs) = default;
    BooleanOperatorToken &operator=(BooleanOperatorToken &&) = default;

    virtual std::vector<size_t> exec() {
        NOT_IMPLEMENTED;
    }
};

struct BooleanOperatorWildcard : public BooleanOperator {
    BooleanOperatorWildcard(BooleanOperatorWildcard &&rhs) = default;
    BooleanOperatorWildcard &operator=(BooleanOperatorWildcard &&) = default;

    virtual std::vector<size_t> exec() {
        NOT_IMPLEMENTED;
    }
};

struct BooleanOperatorPhrase : public BooleanOperator {
    BooleanOperatorPhrase(BooleanOperatorPhrase &&rhs) = default;
    BooleanOperatorPhrase &operator=(BooleanOperatorPhrase &&) = default;

    virtual std::vector<size_t> exec() {
        NOT_IMPLEMENTED;
    }
};

inline std::unique_ptr<BooleanOperator> parse(const std::string &input) {
    NOT_IMPLEMENTED;
}

inline std::vector<size_t>
remove_position(const std::vector<common::DocInvIndexElement> &ind) {
    std::vector<size_t> ret;
    std::transform(ind.begin(), ind.end(), std::back_inserter(ret), [](auto &&x) {
        return x.doc_id;
    });
    return ret;
}

} // namespace ir::ir