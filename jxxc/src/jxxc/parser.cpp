#include "jxxc/parser.hpp"
#include "jxxc/ast/expressions.hpp"
#include "jxxc/tokens.hpp"

namespace jxxc {

std::map<Token::Kind, Parser::ParseRule> Parser::PARSE_RULES{
    { Token::Kind::INT_LITERAL, { Precedence::NONE, &Parser::literal, nullptr }           },
    { Token::Kind::PLUS,        { Precedence::ADDITIVE, &Parser::unary, &Parser::binary } },
    { Token::Kind::MINUS,       { Precedence::ADDITIVE, &Parser::unary, &Parser::binary } },
    { Token::Kind::STAR,        { Precedence::MULTIPLICATIVE, nullptr, &Parser::binary }  },
    { Token::Kind::SLASH,       { Precedence::MULTIPLICATIVE, nullptr, &Parser::binary }  },
    { Token::Kind::L_ROUND,     { Precedence::NONE, &Parser::brackets, nullptr }          },
};

Parser::Parser(SourceText& source, LexemePool& lexemes, ErrorFormatter& errfmt)
    : errfmt{ errfmt }
    , scan{ source, lexemes, errfmt } {

    advance();
    advance();
}

auto Parser::parse(SourceText& source, LexemePool& lexemes, ErrorFormatter& errfmt) -> std::unique_ptr<ast::Expression> {
    return Parser{ source, lexemes, errfmt }.parse();
}

auto Parser::parse() -> std::unique_ptr<ast::Expression> {
    return std::unique_ptr<ast::Expression>{ parseExpression() };
}

auto Parser::parseExpression(int precedence) -> ast::Expression* {

    if (!PARSE_RULES.contains(currentToken.kind) || PARSE_RULES[currentToken.kind].prefix == nullptr) {
        errfmt.expected(currentToken, "a primary expression");
        advance();
        return new ast::InvalidExpression{ previousToken };
    }

    ast::Expression* expr = (this->*PARSE_RULES[currentToken.kind].prefix)();

    while (true) {

        if (!PARSE_RULES.contains(currentToken.kind))
            break;

        ParseRule rule = PARSE_RULES[currentToken.kind];

        if (rule.infix == nullptr)
            break;

        if (rule.precedence <= precedence)
            break;

        expr = (this->*rule.infix)(expr);
    }

    return expr;
}

auto Parser::literal() -> ast::Expression* {
    advance();
    return new ast::LiteralExpression{ previousToken };
}

auto Parser::unary() -> ast::Expression* {
    advance();
    Token operation = previousToken;
    return new ast::UnaryExpression{ operation, parseExpression(Precedence::UNARY) };
}

auto Parser::brackets() -> ast::Expression* {
    advance();
    ast::Expression* expr = parseExpression();
    consume(Token::Kind::R_ROUND, "`)`");
    return expr;
}

auto Parser::binary(ast::Expression* expr) -> ast::Expression* {
    advance();
    Token operation = previousToken;
    int operationPrecedence = PARSE_RULES[operation.kind].precedence;
    return new ast::BinaryExpression{ operation, expr, parseExpression(operationPrecedence) };
}

void Parser::advance() {
    previousToken = currentToken;
    currentToken = nextToken;
    nextToken = scan.next();
}

void Parser::consume(Token::Kind kind, const std::string& message) {
    advance();

    if (previousToken.kind != kind) {
        errfmt.expected(previousToken, "{}", message);
    }
}

} // namespace jxxc
