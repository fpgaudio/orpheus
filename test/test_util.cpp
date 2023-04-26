#include <algorithm>
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "orpheus_util.hpp"
#include <cstdint>
#include <limits>
#include <cmath>
#include <numbers>

TEST_CASE("Test sin32 for positive numbers between 0 and 2pi") {
  for (std::int16_t i = 0; i < std::numeric_limits<int16_t>::max(); i++) {
    const auto actualSine = Math::Fixed::sin(i);
    const auto expectedSine = sin(
      2 * std::numbers::pi *
      static_cast<double>(i) / static_cast<double>(std::numeric_limits<int16_t>::max())
    );

    REQUIRE(static_cast<double>(actualSine) / 4096.0 == Approx(expectedSine).margin(2 / 4096.0));
  }
}

TEST_CASE("Test sin32 for periodicity") {
  for (std::int32_t i = std::numeric_limits<int16_t>::max();
       i < 2 * std::numeric_limits<int16_t>::max(); i++) {
    const auto actualSine = Math::Fixed::sin(i);
    const auto expectedSine = sin(
      2 * std::numbers::pi *
      static_cast<double>(i) / static_cast<double>(std::numeric_limits<int16_t>::max())
    );

    REQUIRE(static_cast<double>(actualSine) / 4096.0 == Approx(expectedSine).margin(2 / 4096.0));
  }
}
