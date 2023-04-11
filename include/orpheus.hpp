#ifndef ORPHEUS_HPP
#define ORPHEUS_HPP

namespace Orpheus {
class Engine {};

class EngineFactory {
public:
  /**
   * @brief Constructs the declared graph.
   */
  static auto build() -> Engine;
};
} // namespace Orpheus

#endif
