#include "orpheus.hpp"
#include "orpheus_util.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>
#include <numeric>
#include <type_traits>

namespace Orpheus {

namespace Graph {

auto Inverse::operator()() const -> Engine::QuantType { return -m_inp(); }
auto Attenuator::operator()() const -> Engine::QuantType {
  const auto input = m_inp();
  const Engine::DoubleQuantType scaled = input * m_attenFactor;
  return static_cast<Engine::QuantType>(
      scaled / std::numeric_limits<AttenFactor>::max());
}
void Attenuator::setAtten(AttenFactor attenuation) {
  m_attenFactor = attenuation;
}

auto SineSource::operator()() const -> Engine::QuantType {
  // std::numeric_limits<int16_t>::max() corresponds to 2pi
  const uint64_t angle =
      (std::numeric_limits<int16_t>::max() * getEngine().getTickCount()) /
      getPeriodTicks();
  // Fixed::Math::sin outputs values [0-4096], and we must output numeric_limits
  // values, hence why we multiply. Could be an easy unsigned shift << 3 with
  // sign carry, but I'll let the compiler do that.
  const auto scale = 8;
  return Math::Fixed::sin(angle) * scale;
}

} // namespace Graph

auto EngineFactory::ofSampleRate(SampleRate rate) -> EngineFactory & {
  m_sampleRate = rate;
  return *this;
}

auto EngineFactory::build() -> Engine {
  return Engine(
      static_cast<std::underlying_type<SampleRate>::type>(m_sampleRate));
}

} // namespace Orpheus
