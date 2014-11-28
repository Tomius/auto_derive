#ifndef OPERATORS_TRIG_TAN_HPP_
#define OPERATORS_TRIG_TAN_HPP_

#include <cmath>
#include "./sin.hpp"
#include "./cos.hpp"
#include "../basic/divide.hpp"

namespace auto_derive {

template<typename Expr>
class Tan : public UnaryOperator<Expr> {
  USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  auto operator()(Args... args) const {
    return tan(expr_(args...));
  }

  friend std::ostream& operator<<(std::ostream& os, Tan const& self) {
    return os << "tan(" << self.expr_ << ')';
  }

  template <typename Variable>
  friend constexpr auto derive(Tan const& tan, Variable v) {
    return 2*derive(tan.expr(), v) / (cos(2 * tan.expr()) + 1);
  }
};

template <typename T>
constexpr std::enable_if_t<!IsZero<T>(), Tan<T>> tan(T t) {
  return Tan<T>(t);
}

template <typename T>
constexpr Zero<T> tan(Zero<T> t) {
  return Zero<T>{};
}


}

#endif
