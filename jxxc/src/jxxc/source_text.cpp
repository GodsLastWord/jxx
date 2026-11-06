#include "jxxc/source_text.hpp"
#include <string>

namespace jxxc {

SourceText::SourceText(const std::string& fileName)
    : fileName{ fileName }
    , source{ fileName, std::ios::binary } {
}

int SourceText::peek() {
    return source.peek();
}

int SourceText::advance() {
    source.get();
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

std::string SourceText::getLineFor(const Position& lexemePosition) {
    std::ios::iostate state = source.rdstate();
    source.clear();

    std::streampos currentPosition = source.tellg();
    source.seekg(lexemePosition.lineStart);

    std::string line;
    std::getline(source, line);

    source.seekg(currentPosition);
    source.setstate(state);
    return line;
}

std::streampos SourceText::absoluteOffset() {
    return source.tellg();
}

} // namespace jxxc
