#pragma once

#include "../tokens.hpp"
#include "../visitor/interfaces.hpp"
#include "forwards.hpp"

namespace jxxc::ast {

struct Expression {
    virtual ~Expression() = default;
    explicit Expression() = default;

    explicit Expression(const Expression&) = delete;
    Expression& operator=(const Expression&) = delete;

    explicit Expression(Expression&&) = delete;
    Expression& operator=(Expression&&) = delete;

    virtual void accept(visitor::ExpressionVisitor&) const = 0;
};

struct InvalidExpression : public Expression {
    const Token error;

    inline explicit InvalidExpression(Token error)
        : error{ error } {
    }

    inline void accept(visitor::ExpressionVisitor& v) const {
        v.visit(*this);
    }
};

struct LiteralExpression : public Expression {
    const Token literal;

    inline explicit LiteralExpression(Token literal)
        : literal{ literal } {
    }

    inline void accept(visitor::ExpressionVisitor& v) const {
        v.visit(*this);
    }
};

struct BinaryExpression : public Expression {
    const Token operation;
    const Expression* leftOperand;
    const Expression* rightOperand;

    inline explicit BinaryExpression(Token operation, const Expression* left, const Expression* right)
        : operation{ operation }
        , leftOperand{ left }
        , rightOperand{ right } {
    }

    inline ~BinaryExpression() {
        delete leftOperand;
        delete rightOperand;
    }

    inline void accept(visitor::ExpressionVisitor& v) const {
        v.visit(*this);
    }
};

struct UnaryExpression : public Expression {
    const Token operation;
    const Expression* operand;

    inline explicit UnaryExpression(Token operation, const Expression* operand)
        : operation{ operation }
        , operand{ operand } {
    }

    inline ~UnaryExpression() {
        delete operand;
    }

    inline void accept(visitor::ExpressionVisitor& v) const {
        v.visit(*this);
    }
};

} // namespace jxxc::ast
