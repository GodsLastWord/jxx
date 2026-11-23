#pragma once

#include <bit>
#include <cstdint>
#include <array>
#include <type_traits>

namespace jxxu {

template<typename T>
    requires std::is_integral_v<T>
auto bytesFrom(T value) -> std::array<std::uint8_t, sizeof(T)> {
    std::array<std::uint8_t, sizeof(T)> bytes;

    value = std::byteswap(value);
    std::memcpy(bytes.data(), &value, sizeof(T));

    return bytes;
}

template<typename T>
auto bytesFrom(T value) -> std::array<std::uint8_t, sizeof(T)> {
    std::array<std::uint8_t, sizeof(T)> bytes;

    std::memcpy(bytes.data(), &value, sizeof(T));

    return bytes;
}

} // namespace jxxu
