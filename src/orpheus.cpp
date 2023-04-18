#include "orpheus.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <numbers>
#include <numeric>
#include <type_traits>

namespace Orpheus {

namespace Graph {

auto Inverse::operator()() const -> Engine::QuantType {
  return static_cast<Engine::QuantType>(-m_inp());
}
auto Attenuator::operator()() const -> Engine::QuantType {
  const auto input = m_inp();
  const Engine::DoubleQuantType scaled = input * m_attenFactor;
  return static_cast<Engine::QuantType>(scaled / std::numeric_limits<AttenFactor>::max());
}
void Attenuator::setAtten(AttenFactor attenuation) {
  m_attenFactor = attenuation;
}

auto SineSource::operator()() const -> Engine::QuantType {
  // TODO(markovejnovic): "SineSource uses floating point math::sin. This should
  // get rewritten."
  // TODO(markovejnovic): SineSource performs a possibly expensive operation
  // that does not get memoized. Wasted.

  const auto tickCount = getEngine().getTickCount();
  const auto samplePeriodNs = getEngine().getSamplePeriodNs();
  const auto samplePeriodS = samplePeriodNs * 1e-9;
  const auto cTime = static_cast<float>(tickCount * samplePeriodNs) * 1e-9;

  const auto frequency =
      1.0F / static_cast<float>(getPeriodTicks() * samplePeriodS);
  const auto ampl =
      static_cast<float>(std::numeric_limits<Engine::QuantType>::max());
  const auto out = static_cast<float>(
      ampl * std::sin(2 * std::numbers::pi * frequency * cTime));

  return static_cast<Engine::QuantType>(out);
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
