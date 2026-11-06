#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace jxxc {

class LexemePool {
    std::vector<std::string> lexemes;

public:
    explicit LexemePool();

    std::size_t add(const std::string&);
    auto get(std::size_t) const -> const std::string&;
};

} // namespace jxxc
