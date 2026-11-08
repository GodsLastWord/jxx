#pragma once

#include <format>
#include <iostream>
#include <print>

#include "lexeme_pool.hpp"
#include "source_text.hpp"
#include "tokens.hpp"

namespace jxxc {

class ErrorFormatter {
    bool errors{ false };

    SourceText& source;
    LexemePool& lexemes;
    const std::string sourceName;

    static const std::string RED;
    static const std::string BOLD_WHITE;
    static const std::string CLEAR;

public:
    explicit ErrorFormatter(SourceText&, LexemePool&);

    template<typename... Arguments>
    static void error(std::format_string<Arguments...> message, Arguments&&... arguments) {
        printExecutable();
        report("error", message, std::forward<Arguments>(arguments)...);
    }

    template<typename... Arguments>
    static void fatal(std::format_string<Arguments...> message, Arguments&&... arguments) {
        printExecutable();
        report("fatal error", message, std::forward<Arguments>(arguments)...);
    }

    template<typename... Arguments>
    void fatal(const Token& token, std::format_string<Arguments...> message, Arguments&&... arguments) {
        std::print("{}:[{}, {}]: ", sourceName, token.position.line, token.position.column);
        report("fatal error", message, std::forward<Arguments>(arguments)...);
    }

    template<typename... Arguments>
    void error(const Token& token, std::format_string<Arguments...> message, Arguments&&... arguments) {
        std::print("{}:[{}, {}]: ", sourceName, token.position.line, token.position.column);

        report("error", message, std::forward<Arguments>(arguments)...);

        std::size_t lineNumberLength = std::to_string(token.position.line).length();

        std::string lineOfCode = source.getLineFor(token.position);
        std::string errorLexeme = lexemes.get(token.lexeme);
        // clang-format off
        std::println(
            std::cerr,
            " {} | {}{}{}{}{}",
            token.position.line,
            lineOfCode.substr(0, token.position.column - 1),
            RED,
            errorLexeme,
            CLEAR,
            lineOfCode.substr(token.position.column + errorLexeme.length() - 1)
        );

        std::println(
            std::cerr,
            " {:>{}} | {}{:>{}}{:~>{}}{}\n",
            "",
            lineNumberLength,
            RED,
            "^",
            token.position.column,
            "",
            errorLexeme.length() - 1,
            CLEAR
        );
        // clang-format on
    }

    template<typename... Arguments>
    void expected(const Token token, std::format_string<Arguments...> message, Arguments&&... arguments) {
        // clang-format off
        error(
            token,
            "expected {}",
            std::format(message, std::forward<Arguments>(arguments)...)
        );
        // clang-format on
    }

private:
    template<typename... Arguments>
    inline static void report(const std::string& type, std::format_string<Arguments...> message, Arguments&&... arguments) {

        // clang-format off
        std::println(
            std::cerr,
            "{}{}:{} {}",
            RED,
            type,
            CLEAR,
            std::format(
                message,
                std::forward<Arguments>(arguments)...
            )
        );
        // clang-format on
    }

    inline static void printExecutable() {
        std::print(std::cerr, "{}jxxc:{} ", BOLD_WHITE, CLEAR);
    }
};

} // namespace jxxc
