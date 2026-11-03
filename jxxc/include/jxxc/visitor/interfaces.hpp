#pragma once

#include "../ast/forwards.hpp"

namespace jxxc::visitor {

struct ExpressionVisitor {
    virtual ~ExpressionVisitor() = default;
    explicit ExpressionVisitor() = default;

    explicit ExpressionVisitor(const ExpressionVisitor&) = delete;
    ExpressionVisitor& operator=(const ExpressionVisitor&) = delete;

    explicit ExpressionVisitor(ExpressionVisitor&&) = delete;
    ExpressionVisitor& operator=(ExpressionVisitor&&) = delete;

    virtual void visit(const ast::Expression&) = 0;
    virtual void visit(const ast::InvalidExpression&) = 0;
    virtual void visit(const ast::LiteralExpression&) = 0;
    virtual void visit(const ast::BinaryExpression&) = 0;
    virtual void visit(const ast::UnaryExpression&) = 0;
};

}
