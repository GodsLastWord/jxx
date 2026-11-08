#include "jxxc/constant_pool.hpp"

#include <algorithm>
#include <format>
#include <stdexcept>
#include <vector>

#include "jxxu/bytecode.hpp"

namespace jxxc {

void ConstantPool::addInt(std::size_t lexeme) {
    entries.emplace_back(ConstantEntry{ jxxu::ConstantType::INT, lexeme });
}

std::uint16_t ConstantPool::indexOf(std::size_t lexeme) const {
    auto it = std::find_if(entries.begin(), entries.end(), [lexeme](auto& p) -> bool {
        return p.lexeme == lexeme;
    });

    if (it != entries.end()) {
        return it - entries.begin();
    }

    throw std::invalid_argument{
        std::format("jxxc::ConstantPool::indexOf: no {} lexeme found", lexeme)
    };
}

auto ConstantPool::constants() const -> const std::vector<ConstantEntry>& {
    return entries;
}

} // namespace jxxc
