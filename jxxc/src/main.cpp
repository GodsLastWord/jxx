#include <filesystem>
#include <iostream>
#include <print>

#include "jxxc/error_formatter.hpp"
#include "jxxc/source_text.hpp"

int main(int argc, char* argv[]) {

    jxxc::ErrorFormatter errfmt;

    if (argc < 2) {
        errfmt.fatal("no input files");
        return 1;
    }

    const std::string fileName{ argv[1] };
    if (!std::filesystem::exists(fileName)) {
        errfmt.fatal("`{}` does not exist", fileName);
        return 1;
    }

    jxxc::SourceText text{ fileName };
    if (!text.isOpen()) {
        errfmt.fatal("`{}` could not be opened", fileName);
        return 1;
    }

    std::println("```");
    for (int c = text.peek(); !text.eof(); c = text.advance()) {
        std::print("{:c}", c);
    }
    std::println("\n```");

    return 0;
}
