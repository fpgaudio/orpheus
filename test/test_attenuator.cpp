#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "orpheus.hpp"
#include "test_utils.hpp"
#include <algorithm>
#include <limits>
#include <vector>

SCENARIO("Sine Waves Can Be Attenuated", "[Attenuator]") {
  GIVEN("A T=1s Sine Wave Graph") {
    auto engine = Orpheus::EngineFactory()
      .ofSampleRate(Orpheus::EngineFactory::SampleRate::kHz48)
      .build();

    const float sinePeriod = 1;
    const auto totalTicks = static_cast<uint32_t>(sinePeriod / time_ns2s(static_cast<float>(engine.getSamplePeriodNs())));

    Orpheus::Graph::SineSource source { &engine };
    source.setPeriodTicks(totalTicks);
    Orpheus::Graph::Attenuator attenuator { &engine, source };
    attenuator.setAtten(std::numeric_limits<Orpheus::Graph::Attenuator::AttenFactor>::max() / 2);
    std::vector<double> sineSamples(totalTicks);
    std::vector<int16_t> attenuatedSamples(totalTicks);

    WHEN("The sine wave generates for a full period") {
      for (size_t i = 1; i < source.getPeriodTicks(); i++) {
        sineSamples[i] = source();
        attenuatedSamples[i] = attenuator();
        engine.tick();
      }
      THEN("The output of the attenuator are half of the sine samples") {
        std::vector<int16_t> halfSineSamples(totalTicks);
        std::transform(sineSamples.cbegin(), sineSamples.cend(), halfSineSamples.begin(),
          [](double inp) -> int16_t { return inp / 2; });
        plot(sineSamples, "Raw Sine");
        plot(attenuatedSamples, "Attenuated Sine");
        // The margin of 1 is necessary since int16_max / 2 is odd, which gets truncated. this is
        // an error of 3 o/oo which is completely acceptable
        REQUIRE_THAT(halfSineSamples, Catch::Approx(attenuatedSamples).margin(1));
      }
    }
  }
}
