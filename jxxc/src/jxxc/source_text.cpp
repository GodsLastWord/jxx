#include "jxxc/source_text.hpp"

namespace jxxc {

SourceText::SourceText(const std::string& fileName)
    : fileName{ fileName }
    , source{ fileName, std::ios::binary } {

    if (isOpen()) {
        lineStartCounter = source.tellg();
    }
}

int SourceText::peek() {
    return source.peek();
}

int SourceText::advance() {

    int current = peek();
    source.get();

    if (current == '\n') {
        lineCounter++;
        columnCounter = 0;
        lineStartCounter = source.tellg();
    }
    columnCounter++;

    return peek();
}

bool SourceText::isOpen() {
    return source.is_open();
}

bool SourceText::eof() {
    return source.eof();
}

std::string SourceText::name() {
    return fileName;
}

std::size_t SourceText::line() {
    return lineCounter;
}

std::size_t SourceText::column() {
    return columnCounter;
}

std::streampos SourceText::lineStart() {
    return lineStartCounter;
}

Position SourceText::position() {
    return { lineCounter, columnCounter, lineStartCounter };
}

std::string SourceText::getLineFor(const Position& lexemePosition) {
    std::ios::iostate state = source.rdstate();
    source.clear();

    std::string line;
    std::getline(source, line);

    source.setstate(state);
    return line;
}

} // namespace jxxc
