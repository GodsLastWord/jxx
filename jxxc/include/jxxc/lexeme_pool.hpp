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
    std::string get(std::size_t) const;
};

} // namespace jxxc
