#pragma once

#include <iostream>
#include <print>

#include "../ast/expressions.hpp"
#include "interfaces.hpp"
#include "jxxc/lexeme_pool.hpp"

namespace jxxc::visitor {

class ASTPrinter : public ExpressionVisitor {
    std::size_t tabDepth{ 0 };
    const std::size_t tabLength{};

    const LexemePool& lexemes;
    std::ostream& out;

public:
    explicit ASTPrinter(const LexemePool& lexemes, std::size_t tabLength = 4, std::ostream& out = std::cout)
        : tabLength{ tabLength }
        , lexemes{ lexemes }
        , out{ out } {
    }

    void visit(const ast::Expression&) override;
    void visit(const ast::InvalidExpression&) override;
    void visit(const ast::LiteralExpression&) override;
    void visit(const ast::BinaryExpression&) override;
    void visit(const ast::UnaryExpression&) override;

public:
    template<typename... Arguments>
    void print(std::format_string<Arguments...> message, Arguments&&... arguments) {
        // clang-format off
        std::println(
            out,
            "{:>{}}{}",
            "",
            tabLength * tabDepth,
            std::format(
                message,
                std::forward<Arguments>(arguments)...
            )
        );
        // clang-format on
    }
};

} // namespace jxxc::visitor
