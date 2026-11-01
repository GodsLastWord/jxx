#include "jxxc/scanner.hpp"

#include <cctype>
#include <utility>

namespace jxxc {

const std::map<std::string, Token::Kind> Scanner::BRACKTES{
    { "(", Token::Kind::L_ROUND },
    { ")", Token::Kind::R_ROUND },
};

const std::map<std::string, Token::Kind> Scanner::OPERATORS{
    { "+", Token::Kind::PLUS  },
    { "-", Token::Kind::MINUS },
    { "*", Token::Kind::STAR  },
    { "/", Token::Kind::SLASH },
};

Scanner::Scanner(SourceText& source, LexemePool& lexemes, ErrorFormatter& errfmt)
    : source{ source }
    , lexemes{ lexemes }
    , errfmt{ errfmt } {
}

Token Scanner::next() {
    Token token;

    switch (getState(source.peek())) {
        case State::ERROR:
            token = scanError();
            break;
        case State::_EOF:
            token = scanEof();
            break;
        case State::WHITESPACE:
            skipWhitespace();
            return next();
        case State::NUMBER:
            token = scanNumber();
            break;
        case State::BRACKET:
            token = scanBracket();
            break;
        case State::OPERATOR:
            token = scanOperator();
            break;
        default:
            std::unreachable();
    }

    if (isErrorToken(token)) {
        handleError(token);
        token = {};
        return next();
    }

    return token;
}

auto Scanner::getState(int c) -> State {

    if (source.eof()) {
        return State::_EOF;
    }

    if (isWhitespace(c)) {
        return State::WHITESPACE;
    }

    if (isDigit(c)) {
        return State::NUMBER;
    }

    if (isBracket(c)) {
        return State::BRACKET;
    }

    if (isOperator(c)) {
        return State::OPERATOR;
    }

    return State::ERROR;
}

auto Scanner::currentState(State state) -> bool {
    return getState(source.peek()) == state;
}

bool Scanner::isWhitespace(int c) {
    return std::isspace(c);
}

bool Scanner::isDigit(int c) {
    return '0' <= c && c <= '9';
}

bool Scanner::isBracket(int c) {
    return BRACKTES.contains(std::string(1, char(c)));
}

bool Scanner::isOperator(int c) {
    return OPERATORS.contains(std::string(1, char(c)));
}

bool Scanner::isErrorToken(const Token& token) {
    switch (token.kind) {
        case Token::Kind::PLACEHOLDER:
        case Token::Kind::ERROR:
        case Token::Kind::UNRECOGNIZED:
        case Token::Kind::INVALID_INT_LITERAL:
            return true;
        default:
            return false;
    }
}

Token Scanner::scanEof() {
    return { Token::Kind::_EOF, lexemes.add(""), source.position() };
}

void Scanner::skipWhitespace() {
    while (isWhitespace(source.peek())) {
        source.advance();
    }
}

Token Scanner::scanNumber() {
    Position startingPosition = source.position();
    std::string lexeme;

    for (int c = source.peek(); getState(c) == State::NUMBER; c = source.advance()) {
        lexeme += char(c);
    }

    return { Token::Kind::INT_LITERAL, lexemes.add(lexeme), startingPosition };
}

Token Scanner::scanBracket() {
    Position startingPosition = source.position();
    std::string lexeme;

    for (int c = source.peek();; c = source.advance()) {
        lexeme += char(c);

        if (!BRACKTES.contains(lexeme)) {
            lexeme.pop_back();
            break;
        }
    }

    return { BRACKTES.at(lexeme), lexemes.add(lexeme), startingPosition };
}

Token Scanner::scanOperator() {
    Position startingPosition = source.position();
    std::string lexeme;

    for (int c = source.peek();; c = source.advance()) {
        lexeme += char(c);

        if (!OPERATORS.contains(lexeme)) {
            lexeme.pop_back();
            break;
        }
    }

    return { OPERATORS.at(lexeme), lexemes.add(lexeme), startingPosition };
}

Token Scanner::scanError() {
    Position startingPosition = source.position();
    std::string lexeme;

    for (int c = source.peek(); getState(c) == State::ERROR; c = source.advance()) {
        lexeme += char(c);
    }

    return { Token::Kind::UNRECOGNIZED, lexemes.add(lexeme), startingPosition };
}

void Scanner::handleError(const Token& error) {
    switch (error.kind) {
        case Token::Kind::ERROR:
            errfmt.error(error, "common error occurred");
            break;
        case Token::Kind::UNRECOGNIZED:
            errfmt.error(error, "unrecognized lexeme was found");
            break;
        case Token::Kind::INVALID_INT_LITERAL:
            errfmt.error(error, "invalid `int` literal format");
            break;
        default:
            errfmt.error(error, "unknown error occurred");
    }
}

} // namespace jxxc
