#pragma once

#include <array>
#include <bit>
#include <cstdint>
#include <type_traits>

namespace jxxu {

template<typename T>
    requires std::is_integral_v<T>
auto bytesTo(const std::array<std::uint8_t, sizeof(T)>& bytes) -> T {
    T value;

    std::memcpy(&value, bytes.data(), sizeof(T));
    value = std::byteswap(value);

    return value;
}

template<typename T>
auto bytesTo(const std::array<std::uint8_t, sizeof(T)>& bytes) -> T {
    T value;

    std::memcpy(&value, bytes.data(), sizeof(T));

    return value;
}

} // namespace jxxu
