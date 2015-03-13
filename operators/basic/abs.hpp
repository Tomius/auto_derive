#ifndef OPERATORS_BASIC_ABS_HPP_
#define OPERATORS_BASIC_ABS_HPP_

#include "./divide.hpp"
#include "../unary_operator.hpp"

namespace auto_derive {

template<typename Expr>
class Abs : public UnaryOperator<Expr> {
  __AUTO_DERIVE_USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  constexpr auto operator()(Args... args) const {
    using std::abs;
    return abs(expr_(args...));
  }

  template<typename Variable>
  friend constexpr auto derive(Abs const& self, Variable v) {
    // abs(f(x))' = sgn(f(x)) * f(x)' = f(x) / |f(x)| * f(x)'
    return (self.expr_ / self) * derive(self.expr_, v);
  }

  friend std::ostream& operator<<(std::ostream& os, Abs const& self) {
    return os << "abs(" << self.expr_ << ')';
  }
};

template<typename Expr>
constexpr enable_if_t<IsFunction<Expr>(), Abs<Expr>> abs(Expr const& expr) {
  return {expr};
}

template<typename T>
constexpr PlusOne<T> abs(PlusOne<T> expr) {
  return expr;
}

template<typename T>
constexpr Zero<T> abs(Zero<T> expr) {
  return expr;
}

template<typename T>
constexpr PlusOne<T> abs(MinusOne<T> expr) {
  return PlusOne<T>{};
}

template<typename T>
constexpr Constant<T> abs(Constant<T> expr) {
  return expr.value > T{} ? expr : Constant<T>{-expr.value};
}

template<typename Expr>
constexpr auto fabs(Expr const& expr) {
  return abs(expr);
}

} // namespace auto_derive

#endif
