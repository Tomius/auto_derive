#ifndef OPERATORS_BASIC_FABS_HPP_
#define OPERATORS_BASIC_FABS_HPP_

#include "../../variable.hpp"
#include "../unary_operator.hpp"
#include "./divide.hpp"

namespace auto_derive {

template<typename Expr>
class Abs : public UnaryOperator<Expr> {
  USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  constexpr auto operator()(Args&&... args) const {
    return std::fabs(expr_(args...));
  }

  template<typename T, const char *str>
  friend constexpr auto gradient(Abs self, Variable<T, str> v) {
    // abs(f(x))' = sgn(f(x)) * f(x)' = f(x) / |f(x)| * f(x)'
    return (self.expr_ / self) * gradient(self.expr_, v);
  }

  friend std::ostream& operator<<(std::ostream& os, Abs const& self) {
    return os << "|" << self.expr_ << "|";
  }
};

template<typename Expr>
constexpr std::enable_if_t<IsFunction<Expr>(), Abs<Expr>> fabs(Expr expr) {
  return {expr};
}

template<typename T>
constexpr PlusOne<T> fabs(PlusOne<T> expr) {
  return expr;
}

template<typename T>
constexpr Zero<T> fabs(Zero<T> expr) {
  return expr;
}

template<typename T>
constexpr PlusOne<T> fabs(MinusOne<T> expr) {
  return PlusOne<T>{};
}

template<typename T>
constexpr Constant<T> fabs(Constant<T> expr) {
  return expr.value > 0 ? expr : Constant<T>{-expr.value};
}

} // namespace auto_derive

#endif
