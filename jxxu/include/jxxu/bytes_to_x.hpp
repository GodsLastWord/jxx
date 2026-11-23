#pragma once

#include <array>
#include <bit>
#include <cstdint>
#include <type_traits>

namespace jxxu {

template<typename T>
auto bytesTo(const std::array<std::uint8_t, sizeof(T)>& bytes) -> T {
    T value;

    std::memcpy(&value, bytes.data(), sizeof(T));

    if constexpr (std::is_integral_v<T>) {
        value = std::byteswap(value);
    }

    return value;
}

} // namespace jxxu
