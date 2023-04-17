#include "orpheus.hpp"
#include "ascii.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include <limits>

template<typename T> auto time_ns2s(T nanosecs) -> float;

template<>
auto time_ns2s(float nanosecs) -> float {
  const float coeff = 1e-9;
  return nanosecs * coeff;
}

template<>
auto time_ns2s(uint64_t nanosecs) -> float {
  const float coeff = 1e-9;
  return static_cast<float>(nanosecs) * coeff;
}

const static std::pair<size_t, size_t> DEFAULT_PLOT_DIMS = { 100, 10 };

template <typename T>
void plot(std::vector<T>& yss, std::pair<size_t, size_t> dims = DEFAULT_PLOT_DIMS) {
  // Have to subsample >:( Graph will look like shit. Ideally we'd sample higher where dy/dx is
  // large to preserve function shape, but you know what, this is supposed to show the library
  // works, not that it works well, so fuck you!
  // I'm not dealing with the rabbit hole that is plotting here. Come back to my gh in 2 years
  // maybe ill get to it.
  std::vector<double> subsamples(dims.first);
  const auto stepSize = yss.size() / dims.first;
  for (size_t i = 0; i < dims.first; i++) {
    subsamples[i] = yss[i * stepSize];
  }

  ascii::Asciichart chart(subsamples);
  std::cout << std::endl << chart.height(static_cast<double>(dims.second)).Plot() << std::endl;
}

SCENARIO("Sine Waves Can Be Generated", "[SineSource]") {
  GIVEN("A T=1s Sine Wave Graph") {
    auto engine = Orpheus::EngineFactory()
      .ofSampleRate(Orpheus::EngineFactory::SampleRate::kHz48)
      .build();

    const float sinePeriod = 1;
    const auto totalTicks = static_cast<uint32_t>(sinePeriod / time_ns2s(static_cast<float>(engine.getSamplePeriodNs())));

    Orpheus::Graph::SineSource source { &engine };
    source.setPeriodTicks(totalTicks);
    std::vector<double> sine(totalTicks);

    WHEN("The tick is 0") {
      THEN("The output of the source must be 0") {
        REQUIRE(source() == 0);
      }
    };

    WHEN("The tick increments by a single period") {
      engine.tick();
      THEN("The output of the source must be non-zero") {
        REQUIRE(source() != 0);
      }
    };

    WHEN("The tick increments to a quarter of the period") {
      for (size_t i = 1; i < source.getPeriodTicks() / 4; i++) {
        sine[i] = source();
        engine.tick();
      }
      THEN("The sinewave reaches its maximum") {
        REQUIRE(std::numeric_limits<Orpheus::Engine::QuantType>::max() == source());
      }
    }

    WHEN("The tick increments to a period") {
      for (size_t i = engine.getTickCount(); i < source.getPeriodTicks(); i++) {
        sine[i] = source();
        engine.tick();
      }
      THEN("The sinewave reaches 0 again") {
        plot(sine);
        REQUIRE(0 == source());
      }
    }
  }
}

SCENARIO("SineSource Performance Tests", "[SineSource]") { 
  GIVEN("A T=1s Sine Wave Graph") {
    auto engine = Orpheus::EngineFactory()
      .ofSampleRate(Orpheus::EngineFactory::SampleRate::kHz48)
      .build();

    const float sinePeriod = 1;
    const auto totalTicks = static_cast<uint32_t>(sinePeriod / time_ns2s(static_cast<float>(engine.getSamplePeriodNs())));

    Orpheus::Graph::SineSource source { &engine };
    source.setPeriodTicks(totalTicks);
    std::vector<double> sine(totalTicks);

    WHEN("The whole sine wave is generated") {
      std::vector<Orpheus::Engine::QuantType> sine(totalTicks);
      std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
      for (size_t i = 0; i < totalTicks; i++) {
        sine[i] = source();
        engine.tick();
      }
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

      THEN("The total time per sample must be less than 20µs") {
        REQUIRE(static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / totalTicks < 20);
        plot(sine);
      }
    }
  }
}
