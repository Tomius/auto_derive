#ifndef OPERATORS_TRIG_TANH_HPP_
#define OPERATORS_TRIG_TANH_HPP_

#include "./sinh.hpp"
#include "./cosh.hpp"
#include "../basic/divide.hpp"
#include "../basic/square.hpp"

namespace auto_derive {

template<typename Expr>
class Tanh : public UnaryOperator<Expr> {
  __AUTO_DERIVE_USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  constexpr auto operator()(Args... args) const {
    return tanh(expr_(args...));
  }

  friend std::ostream& operator<<(std::ostream& os, Tanh const& self) {
    return os << "tanh(" << self.expr_ << ')';
  }

  template <typename Variable>
  friend constexpr auto derive(Tanh const& self, Variable v) {
    return 4*derive(self.expr(), v)*square(cosh(self.expr())) /
           square(cosh(2 * self.expr()) + 1);
  }
};

template <typename T>
constexpr enable_if_t<!IsZero<T>(), Tanh<T>> tanh(T t) {
  return Tanh<T>(t);
}

template <typename T>
constexpr Zero<T> tanh(Zero<T> t) {
  return Zero<T>{};
}


}

#endif
