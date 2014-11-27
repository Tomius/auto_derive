#ifndef OPERATORS_TRIG_ATAN2_HPP_
#define OPERATORS_TRIG_ATAN2_HPP_

#include "./atan.hpp"
#include "../binary_operator.hpp"

namespace auto_derive {

template<typename Lhs, typename Rhs>
class Atan2 : public BinaryOperator<Lhs, Rhs> {
  USING_BINARY_OPERATOR(Lhs, Rhs);

  template<typename... Args>
  auto operator()(Args&&... args) const {
    return std::atan2(lhs_(std::forward<Args>(args)...),
                      rhs_(std::forward<Args>(args)...));
  }

  friend std::ostream& operator<<(std::ostream& os, Atan2 const& self) {
    return os << "atan2(" << self.lhs_ << ',' << self.rhs_ << ')';
  }

  template <typename Variable>
  friend constexpr auto derive(Atan2 const& atan2, Variable v) {
    // At the points where the derivative exists, atan2(x, y) is,
    // except for a constant, equal to atan(y/x).
    return derive(atan2.rhs() / atan2.lhs(), v);
  }
};

template<typename Lhs, typename Rhs>
constexpr auto atan2(Lhs const& lhs, Rhs const& rhs)
    -> std::enable_if_t<!IsZero<Rhs>(), Atan2<Lhs, Rhs>> {
  return {lhs, rhs};
}

template<typename Lhs, typename T>
constexpr Zero<T> atan2(Lhs const& lhs, Zero<T> rhs) {
  return rhs;
}

}

#endif
