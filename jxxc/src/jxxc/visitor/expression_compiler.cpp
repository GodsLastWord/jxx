#include "jxxc/visitor/expression_compiler.hpp"

#include "jxxc/ast/expressions.hpp"
#include "jxxu/bytecode.hpp"

namespace jxxc::visitor {

ExpressionCompiler::ExpressionCompiler(const ConstantPool& constants, const LexemePool& lexemes, ErrorFormatter& errfmt)
    : constants{ constants }
    , lexemes{ lexemes }
    , errfmt{ errfmt }{
}

auto ExpressionCompiler::compile(const ast::Expression& expression, const ConstantPool& constants, const LexemePool& lexemes, ErrorFormatter& errfmt) -> std::vector<std::uint8_t> {
    ExpressionCompiler compiler{ constants, lexemes, errfmt };
    compiler.visit(expression);
    return compiler.bytecode;
}

void ExpressionCompiler::visit(const ast::Expression& expr) {
    expr.accept(*this);
}

void ExpressionCompiler::visit(const ast::InvalidExpression& expr) {
    // . . .
}

void ExpressionCompiler::visit(const ast::LiteralExpression& expr) {
    emitByte(jxxu::BytecodeInstruction::LOAD_CONSTANT);
    emit2Bytes(constants.indexOf(expr.literal.lexeme));
}

void ExpressionCompiler::visit(const ast::BinaryExpression& expr) {
    expr.leftOperand->accept(*this);
    expr.rightOperand->accept(*this);

    switch (expr.operation.kind) {
        case Token::Kind::PLUS:
            emitByte(jxxu::BytecodeInstruction::INT_ADD);
            break;
        case Token::Kind::MINUS:
            emitByte(jxxu::BytecodeInstruction::INT_SUB);
            break;
        case Token::Kind::STAR:
            emitByte(jxxu::BytecodeInstruction::INT_MUL);
            break;
        case Token::Kind::SLASH:
            emitByte(jxxu::BytecodeInstruction::INT_DIV);
            break;
        default:
            errfmt.fatal(expr.operation, "unknown kind `{}` of operation to compile", int(expr.operation.kind));
            return;
    }
}

void ExpressionCompiler::visit(const ast::UnaryExpression& expr) {
    expr.operand->accept(*this);

    switch (expr.operation.kind) {
        case Token::Kind::PLUS:
            break;
        case Token::Kind::MINUS:
            emitByte(jxxu::BytecodeInstruction::INT_NEG);
            break;
        default:
            errfmt.fatal(expr.operation, "unknown kind `{}` of operation to compile", int(expr.operation.kind));
            return;
    }
}

void ExpressionCompiler::emitByte(std::uint8_t byte) {
    bytecode.push_back(byte);
}

void ExpressionCompiler::emit2Bytes(std::uint16_t twoBytes) {
    std::uint8_t* bytes = reinterpret_cast<std::uint8_t*>(&twoBytes);

    bytecode.push_back(bytes[1]);
    bytecode.push_back(bytes[0]);
}

} // namespace jxxc::visitor
