#include "jxxc/lexeme_pool.hpp"

#include <algorithm>
#include <format>
#include <stdexcept>

namespace jxxc {

LexemePool::LexemePool() {
    add("");
}

std::size_t LexemePool::add(const std::string& lexeme) {
    auto it = std::find(lexemes.begin(), lexemes.end(), lexeme);

    if (it != lexemes.end()) {
        return it - lexemes.begin();
    }

    lexemes.push_back(lexeme);
    return lexemes.size() - 1;
}

std::string LexemePool::get(std::size_t index) const {
    if (index >= lexemes.size()) {
        throw std::invalid_argument(
            std::format(
                "jxxc::LexemePool::get: index >= lexemes.size() ({} >= {})",
                index,
                lexemes.size()));
    }

    return lexemes[index];
}

} // namespace jxxc
