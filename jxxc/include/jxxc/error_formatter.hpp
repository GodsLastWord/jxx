#pragma once

#include <format>
#include <iostream>
#include <print>

namespace jxxc {

class ErrorFormatter {
    bool errors{ false };

    std::ostream& out;

    static const std::string RED;
    static const std::string BOLD_WHITE;
    static const std::string CLEAR;

public:
    explicit ErrorFormatter(std::ostream& = std::cerr);

    template<typename... Arguments>
    void error(std::format_string<Arguments...> message, Arguments&&... arguments) {
        printExecutable();

        // clang-format off
        std::println(
            out,
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
    void fatal(std::format_string<Arguments...> message, Arguments&&... arguments) {
        printExecutable();

        // clang-format off
        std::println(
            out,
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
    void printExecutable() {
        std::print(out, "{}jxxc:{} ", BOLD_WHITE, CLEAR);
    }
};

} // namespace jxxc
