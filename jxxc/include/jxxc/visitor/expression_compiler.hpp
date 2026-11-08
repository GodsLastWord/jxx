#pragma once

#include <cstdint>
#include <vector>

#include "interfaces.hpp"
#include "../lexeme_pool.hpp"
#include "../constant_pool.hpp"
#include "../error_formatter.hpp"

namespace jxxc::visitor {

class ExpressionCompiler : private ExpressionVisitor {
    std::vector<std::uint8_t> bytecode;

    const ConstantPool& constants;
    const LexemePool& lexemes;
    ErrorFormatter& errfmt;

    ExpressionCompiler(const ConstantPool&, const LexemePool&, ErrorFormatter&);

public:
    static auto compile(const ast::Expression&, const ConstantPool&, const LexemePool&, ErrorFormatter&) -> std::vector<std::uint8_t>;

private:
    void visit(const ast::Expression&) override;
    void visit(const ast::InvalidExpression&) override;
    void visit(const ast::LiteralExpression&) override;
    void visit(const ast::BinaryExpression&) override;
    void visit(const ast::UnaryExpression&) override;

    void emitByte(std::uint8_t);
    void emit2Bytes(std::uint16_t);
};

} // namespace jxxc::visitor
