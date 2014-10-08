#ifndef OPERATORS_TRIG_SIN_HPP_
#define OPERATORS_TRIG_SIN_HPP_

#include <cmath>
#include "../../variable.hpp"
#include "../unary_operator.hpp"

namespace auto_derive {

template<typename Expr>
class Sin : public UnaryOperator<Expr> {
  USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  auto operator()(Args&&... args) const {
    return std::sin(expr_(std::forward<Args>(args)...));
  }

  static int precendence() { return 1; }

  friend std::ostream& operator<<(std::ostream& os, Sin const& self) {
    return os << "sin(" << self.expr_ << ")";
  }
};

template <typename T>
constexpr std::enable_if_t<!IsZero<T>(), Sin<T>> sin(T t) {
  return Sin<T>(t);
}

template <typename T>
constexpr Zero<T> sin(Zero<T> t) {
  return Zero<T>{};
}

}

#include "./cos.hpp"

namespace auto_derive {

template <typename Expr, typename Variable>
constexpr auto gradient(Sin<Expr> const& sin, Variable v) {
  return cos(sin.expr()) * gradient(sin.expr(), v);
}

}

#endif
