#pragma once

#include <array>
#include <bit>
#include <cstdint>
#include <type_traits>

namespace jxxu {

template<typename T>
auto bytesFrom(T value) -> std::array<std::uint8_t, sizeof(T)> {
    std::array<std::uint8_t, sizeof(T)> bytes;

    if constexpr (std::is_integral_v<T>) {
        value = std::byteswap(value);
    }

    std::memcpy(bytes.data(), &value, sizeof(T));

    return bytes;
}

} // namespace jxxu
