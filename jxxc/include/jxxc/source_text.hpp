#pragma once

#include <fstream>
#include <string>

#include "jxxc/tokens.hpp"

namespace jxxc {

class SourceText {
    const std::string fileName;
    std::ifstream source;

public:
    explicit SourceText(const std::string&);

    int peek();
    int advance();

    bool isOpen();
    bool eof();

    std::string name();

    std::string getLineFor(const Position&);

    std::streampos absoluteOffset();
};

} // namespace jxxc
