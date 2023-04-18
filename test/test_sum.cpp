#include "catch2/catch.hpp"
#include "orpheus.hpp"
#include "test_utils.hpp"
#include <limits>
#include <vector>

SCENARIO("Summing a single sine wave returns the original sine wave", "[Sum]") {
  GIVEN("A Graph of a T=1s SineWave graph feeding into the sum") {
    auto engine = Orpheus::EngineFactory()
      .ofSampleRate(Orpheus::EngineFactory::SampleRate::kHz48)
      .build();

    const float sinePeriod = 1;
    const auto totalTicks = static_cast<uint32_t>(
        sinePeriod / time_ns2s(static_cast<float>(engine.getSamplePeriodNs())));

    Orpheus::Graph::SineSource source { &engine };
    source.setPeriodTicks(totalTicks);
    Orpheus::Graph::Sum sum { &engine, source };

    WHEN("The sine wave generates for a full period") {
      std::vector<double> sineSource(totalTicks);
      std::vector<double> sumResult(totalTicks);
      for (size_t i = 1; i < source.getPeriodTicks(); i++) {
        sineSource[i] = source();
        sumResult[i] = sum();
        engine.tick();
      }
      THEN("The output of the sum matches the output of the sine source") {
        const bool outputsAreEqual = sineSource == sumResult; // Done to prevent array dump with -v
        REQUIRE(outputsAreEqual);
        plot(sineSource, "Sine Source");
        plot(sumResult, "Sum of the Sine Source and nothing else");
      }
    }
  }
}

SCENARIO("Summing multiple sources works as expected.", "[Sum]") {
  GIVEN("A Graph of two T=1s SineWave sources feeding into the sum") {
    auto engine = Orpheus::EngineFactory()
      .ofSampleRate(Orpheus::EngineFactory::SampleRate::kHz48)
      .build();

    const float sinePeriod = 1;
    const auto totalTicks = static_cast<uint32_t>(
        sinePeriod / time_ns2s(static_cast<float>(engine.getSamplePeriodNs())));

    Orpheus::Graph::SineSource source1 { &engine };
    source1.setPeriodTicks(totalTicks);
    Orpheus::Graph::Attenuator atS1 { &engine, source1 };
    atS1.setAtten(std::numeric_limits<Orpheus::Graph::Attenuator::AttenFactor>::max() / 2);
    Orpheus::Graph::SineSource source2 { &engine };
    source2.setPeriodTicks(totalTicks);
    Orpheus::Graph::Attenuator atS2 { &engine, source2 };
    atS2.setAtten(std::numeric_limits<Orpheus::Graph::Attenuator::AttenFactor>::max() / 2);

    Orpheus::Graph::Sum sum { &engine, atS1, atS2 };

    WHEN("The sine waves generate for a full period") {
      std::vector<double> sineSource(totalTicks);
      std::vector<double> sumResult(totalTicks);
      for (size_t i = 1; i < source1.getPeriodTicks(); i++) {
        sineSource[i] = atS1() + atS2();
        sumResult[i] = sum();
        engine.tick();
      }
      THEN("The output of the sum matches the output of the sine source") {
        const bool outputsAreEqual = sineSource == sumResult; // Done to prevent array dump with -v
        REQUIRE(outputsAreEqual);
        plot(sineSource);
        plot(sumResult);
      }
    }
  }
}
