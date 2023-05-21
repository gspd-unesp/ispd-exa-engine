#ifndef ENGINE_MATH_UTILITY_HPP
#define ENGINE_MATH_UTILITY_HPP

#include <core/core.hpp>
#include <cstdint>

/**
 * @brief Szudzik's hash function.
 *
 * @param a the first unsigned 32-bit integer
 * @param b the second unsigned 32-bit integer
 *
 * @return an unsigned 64-bit integer represented the image of the mapping
 *         applied to (a, b)
 */
ENGINE_INLINE static uint64_t szudzik(const uint32_t a, const uint32_t b)
{
    const auto a64 = static_cast<uint64_t>(a);
    const auto b64 = static_cast<uint64_t>(b);
    return a64 >= b64 ? a64 * a64 + a64 + b64 : a64 + b64 * b64;
}

#endif // ENGINE_MATH_UTILITY_HPP
