#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include <string_view>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
#include "ascii.h"
#pragma GCC diagnostic pop
#include <cstdint>
#include <limits>
#include <utility>

template <typename T> auto time_ns2s(T nanosecs) -> float {
  const float coeff = 1e-9;
  return static_cast<float>(nanosecs) * coeff;
}

constexpr static std::pair<size_t, size_t> DEFAULT_PLOT_DIMS = {100, 10};

template <typename T>
void plot(std::vector<T> &yss, const std::string_view title = "",
          std::pair<size_t, size_t> dims = DEFAULT_PLOT_DIMS) {
  // Have to subsample >:( Graph will look like shit. Ideally we'd sample higher
  // where dy/dx is large to preserve function shape, but you know what, this is
  // supposed to show the library works, not that it works well, so fuck you!
  // I'm not dealing with the rabbit hole that is plotting here. Come back to my
  // gh in 2 years maybe ill get to it.
  std::vector<double> subsamples(dims.first);
  const auto stepSize = yss.size() / dims.first;
  for (size_t i = 0; i < dims.first; i++) {
    subsamples[i] = yss[i * stepSize];
  }

  ascii::Asciichart chart(subsamples);
  std::cout << title << std::endl << "-------------" << std::endl
            << chart.height(static_cast<double>(dims.second)).Plot()
            << std::endl;
}

#endif
