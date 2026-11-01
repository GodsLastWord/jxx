#pragma once

#include <format>
#include <iostream>
#include <print>

#include "tokens.hpp"
#include "source_text.hpp"

namespace jxxc {

class ErrorFormatter {
    bool errors{ false };

    SourceText& source;

    static const std::string RED;
    static const std::string BOLD_WHITE;
    static const std::string CLEAR;

public:
    explicit ErrorFormatter(SourceText&);

    template<typename... Arguments>
    static void error(std::format_string<Arguments...> message, Arguments&&... arguments) {
        printExecutable();

        // clang-format off
        std::println(
            std::cerr,
            "{}error:{} {}",
            RED,
            CLEAR,
            std::format(
                message,
                std::forward<Arguments>(arguments)...
            )
        );
        // clang-format on
    }

    template<typename... Arguments>
    void error(const Token& error, std::format_string<Arguments...> message, Arguments&&... arguments) {
        printExecutable();

        // clang-format off
        std::println(
            std::cerr,
            "{}error:{} {}",
            RED,
            CLEAR,
            std::format(
                message,
                std::forward<Arguments>(arguments)...
            )
        );

        std::size_t lineNumberLength = std::to_string(error.position.line).length();
        
        std::println(
            std::cerr,
            " {} | {}",
            error.position.line,
            source.getLineFor(error.position)
        );

        std::println(
            std::cerr,
            " {:>{}} | {:>{}}",
            "",
            lineNumberLength,
            "^",
            error.position.column
        );

        // clang-format on
    }

    template<typename... Arguments>
    static void fatal(std::format_string<Arguments...> message, Arguments&&... arguments) {
        printExecutable();

        // clang-format off
        std::println(
            std::cerr,
            "{}fatal error:{} {}",
            RED,
            CLEAR,
            std::format(
                message,
                std::forward<Arguments>(arguments)...
            )
        );
        // clang-format on
    }

private:
    static void printExecutable() {
        std::print(std::cerr, "{}jxxc:{} ", BOLD_WHITE, CLEAR);
    }
};

} // namespace jxxc
