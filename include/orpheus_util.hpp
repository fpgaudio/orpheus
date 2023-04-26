#ifndef ORPHEUS_UTIL_HPP
#define ORPHEUS_UTIL_HPP

#include <cstdint>
#include <limits>

namespace Math::Fixed {

inline auto bitSet(int16_t value, std::size_t bit) -> bool {
  return static_cast<uint16_t>(value) == (static_cast<uint16_t>(value) | (1U << bit));
}

/**
 * @brief Calculates the sine of the given angle.
 *
 * @param angleRad The fraction of a full circle. A value of INT16_MAX means 2PI.
 *
 * @return 16bit fixed point sine value (4.12)
 */
template<typename T>
inline auto sin(T angleRad) -> int16_t {
  return sin(static_cast<int16_t>(angleRad % std::numeric_limits<int16_t>::max()));
}

template<>
inline auto sin(int16_t angleRad) -> int16_t {
  // Within pi-half. We are only curve-fitting between 0 and pi/2.
  // We therefore ignore higher bits.
  angleRad = static_cast<int16_t>(static_cast<uint16_t>(angleRad) << 1U);

  // Calculate whether we should carry. If the value is greater than pi/2, we will carry.
  const auto carry = angleRad < 0;

  // If the 13th bit is set, that means that this is a value greater than 2^13, implying it is
  // negative. Let us correct that. and invert the bitmask.
  constexpr auto rangeBits = 14U;
  constexpr auto allBits16 = 0xFFFF;
  if (bitSet(angleRad, rangeBits)) {
    angleRad = static_cast<int16_t>(allBits16 - angleRad);
  }

  // Select only the 2^15 bits, shift back down by one spot to correct for the shift at the start.
  constexpr auto allBits15U = 0x7FFFU;
  angleRad = static_cast<int16_t>((static_cast<uint16_t>(angleRad) & allBits15U) >> 1U);

  // Promote to uint32 for future multiplication.
  const auto usAngle = static_cast<uint32_t>(angleRad);

  // Implement the formula
  // y * 2^-n * ( A1 - 2^(q-p)* y * 2^-n * y * 2^-n * [B1 - 2^-r * y * 2^-n * C1 * y]) * 2^(a-q)
  constexpr auto coeffA1 = 3370945099UL;
  constexpr auto coeffB1 = 2746362156UL;
  constexpr auto coeffC1 = 292421UL;
  constexpr auto coeffN = 13U;
  constexpr auto coeffR = 3U;
  constexpr auto coeffP = 32U;
  constexpr auto coeffQ = 31U;
  constexpr auto coeffA = 12U;

  uint32_t out = (coeffC1 * usAngle) >> coeffN;
  out = coeffB1 - ((usAngle * out) >> coeffR);
  out = usAngle * (out >> coeffN);
  out = usAngle * (out >> coeffN);
  out = coeffA1 - (out >> (coeffP - coeffQ));
  out = usAngle * (out >> coeffN);
  out = (out + (1UL << (coeffQ - coeffA - 1UL))) >> (coeffQ - coeffA);

  const auto outCast = carry ? (-static_cast<int16_t>(out)) : static_cast<int16_t>(out);
  return static_cast<int16_t>(outCast);
}

} // namespace Math::Fixed

#endif
