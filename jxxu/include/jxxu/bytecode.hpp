#pragma once

#include <cstddef>
#include <cstdint>

namespace jxxu {

struct BytecodeInstruction {
    enum : std::uint8_t {
        NO_INSTRUCTION,

        LOAD_CONSTANT,

        INT_ADD,
        INT_SUB,
        INT_MUL,
        INT_DIV,
        INT_NEG
    };
};

struct ConstantPool {
    enum : std::uint8_t {
        NO_CONSTANT,

        INT,
    };
};

} // namespace jxxu

namespace jxxu::info {

constexpr std::uint32_t MAGIC_NUMBER{ 0xDEAD'C0DE };

constexpr std::uint16_t MAJOR_VERSION{ 0 };
constexpr std::uint16_t MINOR_VERSION{ 0 };

constexpr std::size_t HEADER_SIZE{
    sizeof(MAGIC_NUMBER)  +
    sizeof(MAJOR_VERSION) +
    sizeof(MINOR_VERSION)
};

} // namespace jxxu::info
