#include <cmath>
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "orpheus.hpp"
#include "test_utils.hpp"

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

    const auto initialTicks = 10;
    WHEN("The tick increments by a couple periods") {
      for (size_t i = 0; i < initialTicks; i++) { engine.tick(); }
      THEN("The output of the source must be non-zero") {
        REQUIRE(source() != 0);
      }
    };

    WHEN("The tick increments to a quarter of the period") {
      for (size_t i = initialTicks; i < source.getPeriodTicks() / 4; i++) {
        sine[i] = source();
        engine.tick();
      }
      THEN("The sinewave reaches its maximum") {
        REQUIRE(std::pow(2, 16) == source());
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
