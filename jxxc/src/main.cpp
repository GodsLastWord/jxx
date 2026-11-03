#include <filesystem>

#include "jxxc/error_formatter.hpp"
#include "jxxc/lexeme_pool.hpp"
#include "jxxc/parser.hpp"
#include "jxxc/source_text.hpp"
#include "jxxc/visitor/ast_printer.hpp"

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
    jxxc::visitor::ASTPrinter printer{ lexemes };

    auto tree = jxxc::Parser::parse(text, lexemes, errfmt);

    printer.visit(*tree);

    return 0;
}
