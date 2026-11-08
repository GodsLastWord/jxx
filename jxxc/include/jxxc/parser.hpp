#pragma once

#include <memory>
#include <map>

#include "ast/expressions.hpp"
#include "error_formatter.hpp"
#include "lexeme_pool.hpp"
#include "scanner.hpp"
#include "source_text.hpp"
#include "tokens.hpp"
#include "constant_pool.hpp"

namespace jxxc {

class Parser {
    using PrefixFunction = auto (Parser::*)() -> ast::Expression*;
    using InfixFunction = auto (Parser::*)(ast::Expression*) -> ast::Expression*;

    struct Precedence {
        enum : int {
            NONE,
            ADDITIVE,
            MULTIPLICATIVE,
            UNARY,
        };
    };

    struct ParseRule {
        const int precedence{ Precedence::NONE };
        const PrefixFunction prefix{ nullptr };
        const InfixFunction infix{ nullptr };
    };

    static std::map<Token::Kind, ParseRule> PARSE_RULES;

    ErrorFormatter& errfmt;
    ConstantPool& constants;
    Scanner scan;

    Token previousToken;
    Token currentToken;
    Token nextToken;

public:
    explicit Parser(SourceText&, LexemePool&, ConstantPool&, ErrorFormatter&);

    auto parse() -> std::unique_ptr<ast::Expression>;
    static auto parse(SourceText&, LexemePool&, ConstantPool&, ErrorFormatter&) -> std::unique_ptr<ast::Expression>;

private:
    auto parseExpression(int = Precedence::NONE) -> ast::Expression*;

    auto literal() -> ast::Expression*;
    auto unary() -> ast::Expression*;
    auto brackets() -> ast::Expression*;
    auto binary(ast::Expression*) -> ast::Expression*;

    void advance();
    void consume(Token::Kind, const std::string&);
};

} // namespace jxxc
