#pragma once

#include <fstream>
#include <string>

#include "jxxc/tokens.hpp"

namespace jxxc {

class SourceText {
    const std::string fileName;
    std::ifstream source;

    std::size_t lineCounter{ 1 };
    std::size_t columnCounter{ 1 };
    std::streampos lineStartCounter{};

public:
    explicit SourceText(const std::string&);

    int peek();
    int advance();

    bool isOpen();
    bool eof();

    std::string name();

    std::size_t line();
    std::size_t column();
    std::streampos lineStart();
    Position position();

    std::string getLineFor(const Position&);
};

} // namespace jxxc
