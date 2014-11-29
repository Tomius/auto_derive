#ifndef OPERATORS_TRIG_ATAN_HPP_
#define OPERATORS_TRIG_ATAN_HPP_

#include <cmath>
#include "../basic/divide.hpp"
#include "../basic/square.hpp"
#include "../basic/subtract.hpp"
#include "../basic/unary_minus.hpp"
#include "../unary_operator.hpp"

namespace auto_derive {

template<typename Expr>
class Atan : public UnaryOperator<Expr> {
  __AUTO_DERIVE_USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  auto operator()(Args... args) const {
    return atan(expr_(args...));
  }

  friend std::ostream& operator<<(std::ostream& os, Atan const& self) {
    return os << "atan(" << self.expr_ << ')';
  }

  template <typename Variable>
  friend constexpr auto derive(Atan const& atan, Variable v) {
    return derive(atan.expr(), v) / (square(atan.expr()) + 1);
  }
};

template <typename T>
constexpr std::enable_if_t<!IsZero<T>(), Atan<T>> atan(T t) {
  return Atan<T>(t);
}

template <typename T>
constexpr Zero<T> atan(Zero<T> t) {
  return Zero<T>{};
}

}

#endif
