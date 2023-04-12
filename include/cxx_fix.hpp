#ifndef CXX_FIX_HPP
#define CXX_FIX_HPP

#include <tuple>

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#define NOCOPY(ClassName)                                                      \
  ClassName(const ClassName &) = delete;                                       \
  auto operator=(const ClassName &)->ClassName & = delete

#define NOMOVE(ClassName)                                                      \
  ClassName(const ClassName &&) = delete;                                      \
  auto operator=(const ClassName &&)->ClassName && = delete

#define NODEFAULT(ClassName)                                                   \
  ClassName() = delete;                                                        \
  virtual ~ClassName() = default

#define NODEFAULT_NOCOPY_NOMOVE(ClassName)                                     \
  NODEFAULT(ClassName);                                                        \
  NOCOPY(ClassName);                                                           \
  NOMOVE(ClassName)
// NOLINTEND(cppcoreguidelines-macro-usage)

template <class Tuple> auto tuple_sum(Tuple const &tuple) -> decltype(auto) {
  auto sum_them = [](auto const &...ele) -> decltype(auto) {
    return (ele + ...);
  };
  return std::apply(sum_them, tuple);
};

#endif
