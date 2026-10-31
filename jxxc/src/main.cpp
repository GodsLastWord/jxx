#include <filesystem>
#include <print>
#include <iostream>

#include "jxxc/source_text.hpp"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::println(std::cerr, "jxxc: fatal error: no input files");
        return 1;
    }

    const std::string fileName{ argv[1] };
    if (!std::filesystem::exists(fileName)) {
        std::println(std::cerr, "jxxc: fatal error: `{}` does not exist", fileName);
        return 1;
    }

    jxxc::SourceText text{ fileName };
    if (!text.isOpen()) {
        std::println(std::cerr, "jxxc: fatal error: `{}` could not be opened", fileName);
        return 1;
    }

    std::println("```");
    for (int c = text.peek(); !text.eof(); c = text.advance()) {
        std::print("{:c}", c);
    }
    std::println("\n```");

    return 0;
}
