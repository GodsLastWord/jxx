#pragma once

#include <cstddef>
#include <iosfwd>

namespace jxxc {

struct Position {
    std::size_t line{ 0 };
    std::size_t column{ 0 };
    std::streampos lineStart{};

    Position() = default;

    Position(std::size_t line, std::size_t column, std::streampos lineStart)
        : line{ line }
        , column{ column }
        , lineStart{ lineStart } {
    }
};

struct Token {
    enum class Kind {
        PLACEHOLDER,

        ERROR,
        UNRECOGNIZED,
        INVALID_INT_LITERAL,

        _EOF,

        INT_LITERAL,

        PLUS,
        MINUS,
        STAR,
        SLASH,

        L_ROUND,
        R_ROUND,
    };

    Kind kind{ Kind::PLACEHOLDER };
    std::size_t lexeme{ 0 };
    Position position{};

    Token() = default;

    Token(Kind kind, std::size_t lexeme, Position position)
        : kind{ kind }
        , lexeme{ lexeme }
        , position{ position } {
    }
};

} // namespace jxxc
