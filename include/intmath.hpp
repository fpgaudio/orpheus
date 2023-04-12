#ifndef INTMATH_HPP
#define INTMATH_HPP

#include <cmath>
#include <cstdint>

namespace Orpheus::Math::Approx {

/**
 * @brief Calculates the sine of a given value exclusively as an integer with resulting values in
 * [std::numeric_limits<T>::min(), std::numeric_limits<T>::max()].
 *
 * @param inputRadNs The input time in nanoseconds.
 *
 * @note This is not a hyper-accurate sine, and the error is about 0.01%.
 */
template<typename T>
auto sin(T inputNs) -> T {
  // Lots of magic numbers, but long story short, this is an optimal solution for taylor series of
  // n = 4 approximation.
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  // TODO(markovejnovic): I'm not so sure about the 98 offset.
  return 0
    + 931 * std::pow(inputNs - 98, 4)
    - 3996 * std::pow(inputNs - 98, 3)
    - 11168 * std::pow(inputNs - 98, 2)
    + 23975 * inputNs 
    + 4354;
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
}

} // namespace Orpheus::Math::Approx

#endif
