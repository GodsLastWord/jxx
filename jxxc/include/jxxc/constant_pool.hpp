#pragma once

#include <cstdint>
#include <vector>

namespace jxxc {

class ConstantPool {
    struct ConstantEntry {
        const std::uint8_t type;
        const std::size_t lexeme;
    };

    std::vector<ConstantEntry> entries;

public:
    void addInt(std::size_t);
    std::uint16_t indexOf(std::size_t) const;
    auto constants() const -> const std::vector<ConstantEntry>&;
};

} // namespace jxxc
