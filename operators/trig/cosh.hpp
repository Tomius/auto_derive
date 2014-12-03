#ifndef OPERATORS_TRIG_COSH_HPP_
#define OPERATORS_TRIG_COSH_HPP_

#include <cmath>
#include "../unary_operator.hpp"

namespace auto_derive {

template<typename Expr>
class Cosh : public UnaryOperator<Expr> {
  __AUTO_DERIVE_USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  constexpr auto operator()(Args... args) const {
    return cosh(expr_(args...));
  }

  friend std::ostream& operator<<(std::ostream& os, Cosh const& self) {
    return os << "cosh(" << self.expr_ << ')';
  }
};

template <typename T>
constexpr std::enable_if_t<!IsZero<T>(), Cosh<T>> cosh(T t) {
  return Cosh<T>(t);
}

template <typename T>
constexpr PlusOne<T> cosh(Zero<T> t) {
  return PlusOne<T>{};
}

}

#include "./sinh.hpp"
#include "../basic/multiply.hpp"
#include "../basic/unary_minus.hpp"

namespace auto_derive {

template <typename Expr, typename Variable>
constexpr auto derive(Cosh<Expr> const& self, Variable v) {
  return sinh(self.expr()) * derive(self.expr(), v);
}

}

#endif
