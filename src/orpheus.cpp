#include "orpheus.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <numbers>
#include <numeric>
#include <type_traits>
#include "intmath.hpp"

namespace Orpheus {

namespace Graph {

auto Inverse::operator()() const -> Engine::QuantType {
  return static_cast<Engine::QuantType>(-m_inp());
}
auto Attenuator::operator()() const -> Engine::QuantType {
  return static_cast<Engine::QuantType>(
      m_attenFactor * m_inp() / std::numeric_limits<AttenFactor>::max());
}
void Attenuator::setAtten(AttenFactor attenuation) {
  m_attenFactor = attenuation;
}

auto SineSource::operator()() const -> Engine::QuantType {
  // TODO(markovejnovic): SineSource performs a possibly expensive operation
  // that does not get memoized. Wasted.

  return static_cast<Engine::QuantType>(Orpheus::Math::Approx::sin(getEngine().getTickCount()));
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
