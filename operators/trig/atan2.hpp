#ifndef OPERATORS_TRIG_ATAN2_HPP_
#define OPERATORS_TRIG_ATAN2_HPP_

#include "./atan.hpp"
#include "../binary_operator.hpp"

namespace auto_derive {

template<typename Lhs, typename Rhs>
class Atan2 : public BinaryOperator<Lhs, Rhs> {
  __AUTO_DERIVE_USING_BINARY_OPERATOR(Lhs, Rhs);

  template<typename... Args>
  constexpr auto operator()(Args... args) const {
    return atan2(lhs_(args...), rhs_(args...));
  }

  friend std::ostream& operator<<(std::ostream& os, Atan2 const& self) {
    return os << "atan2(" << self.lhs_ << ',' << self.rhs_ << ')';
  }

  template <typename Variable>
  friend constexpr auto derive(Atan2 const& self, Variable v) {
    // At the points where the derivative exists, atan2(y, x) is,
    // except for a constant, equal to atan(y/x).
    return derive(atan(self.lhs() / self.rhs()), v);
  }
};

template<typename Lhs, typename Rhs>
constexpr auto atan2(Lhs const& lhs, Rhs const& rhs)
    -> std::enable_if_t<!IsZero<Lhs>(), Atan2<Lhs, Rhs>> {
  return {lhs, rhs};
}

template<typename T, typename Rhs>
constexpr Zero<T> atan2(Zero<T> lhs, Rhs const& rhs) {
  return lhs;
}

}

#endif
