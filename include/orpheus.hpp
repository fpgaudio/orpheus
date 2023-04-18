#ifndef ORPHEUS_HPP
#define ORPHEUS_HPP

#include "cxx_fix.hpp"
#include <cstdint>
#include <format>
#include <initializer_list>
#include <limits>
#include <numeric>
#include <string>
#include <tuple>
#include <type_traits>

namespace Orpheus {

class Engine {
public:
  using SampleType = uint32_t;
  using TickType = uint64_t;
  using QuantType = int16_t;
  using DoubleQuantType = int32_t;

  NODEFAULT_NOCOPY_NOMOVE(Engine);

  explicit constexpr Engine(SampleType samplePeriod)
      : m_samplePeriodNs(samplePeriod) {}

  [[nodiscard]] constexpr auto getSamplePeriodNs() const -> SampleType {
    return m_samplePeriodNs;
  };
  [[nodiscard]] constexpr auto getTickCount() const -> TickType {
    return m_tick;
  };
  constexpr void tick() { m_tick++; }

private:
  TickType m_tick = 0;
  const SampleType m_samplePeriodNs;
};

namespace Graph {

class Node {
public:
  NODEFAULT_NOCOPY_NOMOVE(Node);

  constexpr explicit Node(const Engine *engine) : m_engine(engine){};

  /**
   * The operator() is called to sample a node with the given sample rate.
   */
  virtual auto operator()() const -> Engine::QuantType = 0;

protected:
  [[nodiscard]] constexpr auto getEngine() const -> const Engine & {
    return *m_engine;
  }

private:
  const Engine *m_engine;
};

class WithVhdl {
public:
  virtual auto operator!() const -> std::string = 0;
};

template <typename... T> class Sum final : public Node, public WithVhdl {
public:
  NODEFAULT_NOCOPY_NOMOVE(Sum);
  constexpr explicit Sum(const Engine *engine, const T &...nodes)
      : Node(engine), m_inps(nodes...) {}

  auto operator()() const -> Engine::QuantType override {
    return tuple_sum(m_inps, [](auto const &...ele) -> decltype(auto) {
      return (ele() + ...);
    });
  };

  auto operator!() const -> std::string override {
    const char* inputParam = "inp0, inp1, inp2";
    const char* inputDeclaration = "input inp0; input inp1; input inp2;";
    const char* sumExpr = "inp0 + inp1 + inp2";
    return std::format(R"HERE(
      module orpheus_sum_%d(%s, o)
        %s
        output o;

        o <= %s;
      endmodule
    )HERE", inputParam, inputDeclaration, sumExpr);
  }

private:
  std::tuple<const T &...> m_inps;
};

class Inverse final : public Node, public WithVhdl {
public:
  NODEFAULT_NOCOPY_NOMOVE(Inverse);
  constexpr explicit Inverse(const Engine *engine, const Node &inp)
      : Node(engine), m_inp(inp) {}

  auto operator()() const -> Engine::QuantType override;
  auto operator!() const -> std::string override;

private:
  const Node &m_inp;
};

class Attenuator final : public Node {
public:
  using AttenFactor = int16_t;

  NODEFAULT_NOCOPY_NOMOVE(Attenuator);
  constexpr explicit Attenuator(const Engine *engine, const Node &inp)
      : Node(engine), m_inp(inp) {}
  auto operator()() const -> Engine::QuantType override;
  void setAtten(AttenFactor attenuation);

private:
  const Node &m_inp;
  AttenFactor m_attenFactor = std::numeric_limits<AttenFactor>::max();
};

class Source : public Node {
public:
  NODEFAULT_NOCOPY_NOMOVE(Source);

  constexpr explicit Source(const Engine *engine) : Node(engine) {}
  constexpr void setPeriodTicks(Engine::SampleType period) {
    m_period = period;
  }
  [[nodiscard]] constexpr auto getPeriodTicks() const -> Engine::SampleType {
    return m_period;
  }

private:
  Engine::SampleType m_period = 0;
};

class SineSource final : public Source {
public:
  NODEFAULT_NOCOPY_NOMOVE(SineSource);

  constexpr explicit SineSource(const Engine *engine) : Source(engine) {}
  auto operator()() const -> Engine::QuantType override;
};

} // namespace Graph

class EngineFactory {
public:
  enum class SampleRate : Engine::SampleType {
    kHz48 = 20833 // 20833ns is 48khz
  };

  auto build() -> Engine;
  auto ofSampleRate(SampleRate rate) -> EngineFactory &;

private:
  SampleRate m_sampleRate;
};
} // namespace Orpheus

#endif
