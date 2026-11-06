#pragma once

#include <map>
#include <string>

#include "jxxc/error_formatter.hpp"
#include "jxxc/lexeme_pool.hpp"
#include "jxxc/source_text.hpp"
#include "tokens.hpp"

namespace jxxc {

class Scanner {

    enum class State {
        ERROR,
        _EOF,
        WHITESPACE,
        NUMBER,
        BRACKET,
        OPERATOR,
    };

    static const std::map<std::string, Token::Kind> BRACKTES;
    static const std::map<std::string, Token::Kind> OPERATORS;

    SourceText& source;
    LexemePool& lexemes;
    ErrorFormatter& errfmt;

    std::size_t lineCounter{ 1 };
    std::size_t columnCounter{ 1 };
    std::streampos lineStartCounter{};

public:
    explicit Scanner(SourceText&, LexemePool&, ErrorFormatter&);

    Token next();

private:
    auto getState(int) -> State;
    auto currentState(State) -> bool;

    static bool isWhitespace(int);
    static bool isDigit(int);
    static bool isBracket(int);
    static bool isOperator(int);

    static bool isErrorToken(const Token&);

    Token scanEof();
    void skipWhitespace();
    Token scanNumber();
    Token scanBracket();
    Token scanOperator();
    Token scanError();

    void handleError(const Token&);

    int peek();
    int advance();
    Position currentPosition();
};

} // namespace jxxc
