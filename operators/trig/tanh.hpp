#ifndef OPERATORS_TRIG_TANH_HPP_
#define OPERATORS_TRIG_TANH_HPP_

#include <cmath>
#include "./sinh.hpp"
#include "./cosh.hpp"
#include "../basic/divide.hpp"
#include "../basic/square.hpp"

namespace auto_derive {

template<typename Expr>
class Tanh : public UnaryOperator<Expr> {
  USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  auto operator()(Args... args) const {
    return tanh(expr_(args...));
  }

  friend std::ostream& operator<<(std::ostream& os, Tanh const& self) {
    return os << "tanh(" << self.expr_ << ')';
  }

  template <typename Variable>
  friend constexpr auto derive(Tanh const& tanh, Variable v) {
    return 4*derive(tanh.expr(), v)*square(cosh(tanh.expr())) /
           square(cosh(2 * tanh.expr()) + 1);
  }
};

template <typename T>
constexpr std::enable_if_t<!IsZero<T>(), Tanh<T>> tanh(T t) {
  return Tanh<T>(t);
}

template <typename T>
constexpr Zero<T> tanh(Zero<T> t) {
  return Zero<T>{};
}


}

#endif
