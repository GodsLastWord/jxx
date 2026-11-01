#include <filesystem>
#include <iostream>
#include <print>
#include <vector>

#include "jxxc/error_formatter.hpp"
#include "jxxc/lexeme_pool.hpp"
#include "jxxc/scanner.hpp"
#include "jxxc/source_text.hpp"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        jxxc::ErrorFormatter::fatal("no input files");
        return 1;
    }

    const std::string fileName{ argv[1] };
    if (!std::filesystem::exists(fileName)) {
        jxxc::ErrorFormatter::fatal("`{}` does not exist", fileName);
        return 1;
    }

    jxxc::SourceText text{ fileName };
    if (!text.isOpen()) {
        jxxc::ErrorFormatter::fatal("`{}` could not be opened", fileName);
        return 1;
    }

    jxxc::ErrorFormatter errfmt{ text };
    jxxc::LexemePool lexemes;
    jxxc::Scanner scan{ text, lexemes, errfmt };

    std::vector<jxxc::Token> correctTokens;
    for (jxxc::Token t = scan.next(); t.kind != jxxc::Token::Kind::_EOF; t = scan.next()) {
        correctTokens.push_back(t);
    }

    for (auto& t : correctTokens) {
        std::println("{:>5}[{:>2}, {:>2}] -> `{}`", int(t.kind), t.position.line, t.position.column, lexemes.get(t.lexeme));
    }

    return 0;
}
