#include "jxxc/visitor/ast_printer.hpp"

namespace jxxc::visitor {

void ASTPrinter::visit(const ast::Expression& expr) {
    expr.accept(*this);
}

void ASTPrinter::visit(const ast::InvalidExpression& expr) {
    print("(invalid `{}`)", lexemes.get(expr.error.lexeme));
}

void ASTPrinter::visit(const ast::LiteralExpression& expr) {
    print("(literal `{}`)", lexemes.get(expr.literal.lexeme));
}

void ASTPrinter::visit(const ast::BinaryExpression& expr) {
    print("(binary `{}`", lexemes.get(expr.operation.lexeme));

    tabDepth++;
    expr.leftOperand->accept(*this);
    expr.rightOperand->accept(*this);
    tabDepth--;

    print(")");
}

void ASTPrinter::visit(const ast::UnaryExpression& expr) {
    print("(unary `{}`", lexemes.get(expr.operation.lexeme));

    tabDepth++;
    expr.operand->accept(*this);
    tabDepth--;

    print(")");
}

} // namespace jxxc::visitor
