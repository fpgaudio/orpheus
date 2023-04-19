#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "orpheus.hpp"
#include "test_utils.hpp"
#include <algorithm>
#include <limits>
#include <vector>

SCENARIO("Sine Waves Can Be Inverted", "[Inverse]") {
  GIVEN("A T=1s Sine Wave Graph") {
    auto engine = Orpheus::EngineFactory()
      .ofSampleRate(Orpheus::EngineFactory::SampleRate::kHz48)
      .build();

    const float sinePeriod = 1;
    const auto totalTicks = static_cast<uint32_t>(sinePeriod / time_ns2s(static_cast<float>(engine.getSamplePeriodNs())));

    Orpheus::Graph::SineSource source { &engine };
    source.setPeriodTicks(totalTicks);
    Orpheus::Graph::Inverse inverse { &engine, source };
    std::vector<double> sineSamples(totalTicks);
    std::vector<double> invertedSamples(totalTicks);

    WHEN("The sine wave generates for a full period") {
      for (size_t i = 1; i < source.getPeriodTicks(); i++) {
        sineSamples[i] = source();
        invertedSamples[i] = inverse();
        engine.tick();
      }
      THEN("The output of the inverse are -sine") {
        std::vector<double> invertedSineSamples(totalTicks);
        std::transform(sineSamples.cbegin(), sineSamples.cend(), invertedSineSamples.begin(),
          [](double inp) { return -inp; });
        plot(sineSamples, "Raw Sine");
        plot(invertedSamples, "Inverse Sine");
        const bool arrsEq = invertedSamples == invertedSineSamples;
        REQUIRE(arrsEq);
      }
    }
  }
}
