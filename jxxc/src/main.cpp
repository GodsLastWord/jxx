#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>

#include "jxxc/constant_pool.hpp"
#include "jxxc/error_formatter.hpp"
#include "jxxc/lexeme_pool.hpp"
#include "jxxc/parser.hpp"
#include "jxxc/source_text.hpp"
#include "jxxc/visitor/ast_printer.hpp"
#include "jxxc/visitor/expression_compiler.hpp"

#include "jxxu/bytecode.hpp"
#include "jxxu/bytes_from_x.hpp"

std::vector<std::uint8_t> compileConstantPool(const jxxc::ConstantPool& constantPool, const jxxc::LexemePool& lexemes) {
    std::vector<std::uint8_t> bytecode;

    auto constantNumber = jxxu::bytesFrom<std::uint32_t>(constantPool.constants().size());
    // bytecode.append_range(constantNumber);
    bytecode.insert(bytecode.end(), constantNumber.begin(), constantNumber.end());

    for (auto& constant : constantPool.constants()) {
        bytecode.push_back(constant.type);

        switch (constant.type) {
            case jxxu::ConstantType::INT: {
                auto bytes = jxxu::bytesFrom<std::int32_t>(std::stoi(lexemes.get(constant.lexeme)));
                // bytecode.append_range(bytes);
                bytecode.insert(bytecode.end(), bytes.begin(), bytes.end());
            }
        }
    }

    return bytecode;
}

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

    jxxc::LexemePool lexemes;
    jxxc::ConstantPool constantPool;
    jxxc::ErrorFormatter errfmt{ text, lexemes };
    jxxc::visitor::ASTPrinter printer{ lexemes };

    auto tree = jxxc::Parser::parse(text, lexemes, constantPool, errfmt);

    printer.visit(*tree);

    {
        std::ofstream file{ fileName + ".bxx", std::ios::binary };

        if (!file.is_open()) {
            jxxc::ErrorFormatter::fatal("could not create `{}`", fileName + ".bxx");
            return -1;
        }

        auto magicNumber = jxxu::bytesFrom(jxxu::info::MAGIC_NUMBER);
        auto majorVersion = jxxu::bytesFrom(jxxu::info::MAJOR_VERSION);
        auto minorVersion = jxxu::bytesFrom(jxxu::info::MINOR_VERSION);
        std::vector<std::uint8_t> constantPoolBytecode = compileConstantPool(constantPool, lexemes);
        std::vector<std::uint8_t> bytecode = jxxc::visitor::ExpressionCompiler::compile(*tree, constantPool, lexemes, errfmt);
        auto bytecodeSize = jxxu::bytesFrom<std::uint32_t>(bytecode.size());

        file.write(reinterpret_cast<const char*>(magicNumber.data()), magicNumber.size());                   // magic number
        file.write(reinterpret_cast<const char*>(majorVersion.data()), majorVersion.size());                 // major version
        file.write(reinterpret_cast<const char*>(minorVersion.data()), minorVersion.size());                 // minor version
        file.write(reinterpret_cast<const char*>(constantPoolBytecode.data()), constantPoolBytecode.size()); // constant pool (size is added in compileConstantPool)
        file.write(reinterpret_cast<const char*>(bytecodeSize.data()), bytecodeSize.size());                      // bytecode size
        file.write(reinterpret_cast<const char*>(bytecode.data()), bytecode.size());                         // bytecode
    }

    return 0;
}
