#pragma once

#include <complex>
#include <format>
#include <iostream>
#include <print>

#include "source_text.hpp"
#include "tokens.hpp"

namespace jxxc {

class ErrorFormatter {
    bool errors{ false };

    SourceText& source;
    const std::string sourceName;

    static const std::string RED;
    static const std::string BOLD_WHITE;
    static const std::string CLEAR;

public:
    explicit ErrorFormatter(SourceText&);

    template<typename... Arguments>
    static void error(std::format_string<Arguments...> message, Arguments&&... arguments) {
        printExecutable();

        print("error", message, arguments...);
    }

    template<typename... Arguments>
    static void fatal(std::format_string<Arguments...> message, Arguments&&... arguments) {
        printExecutable();
        print("fatal error", message, arguments...);
    }

    template<typename... Arguments>
    void error(const Token& token, std::format_string<Arguments...> message, Arguments&&... arguments) {
        printSourceName();
        std::print("[{}, {}]: ", token.position.line, token.position.column);

        print("error", message, std::forward<Arguments>(arguments)...);

        std::size_t lineNumberLength = std::to_string(token.position.line).length();

        // clang-format off
        std::println(
            std::cerr,
            " {} | {}",
            token.position.line,
            source.getLineFor(token.position)
        );

        std::println(
            std::cerr,
            " {:>{}} | {:>{}}\n",
            "",
            lineNumberLength,
            "^",
            token.position.column
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
    inline static void print(const std::string& type, std::format_string<Arguments...> message, Arguments&&... arguments) {

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

    inline void printSourceName() {
        std::print(std::cerr, "{}:", sourceName);
    }

    inline static void printExecutable() {
        std::print(std::cerr, "{}jxxc:{} ", BOLD_WHITE, CLEAR);
    }
};

} // namespace jxxc
