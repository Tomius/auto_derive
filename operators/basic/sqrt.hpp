#ifndef OPERATORS_BASIC_SQRT_HPP_
#define OPERATORS_BASIC_SQRT_HPP_

#include "../unary_operator.hpp"

namespace auto_derive {

template<typename Expr>
class Sqrt : public UnaryOperator<Expr> {
  USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  constexpr auto operator()(Args... args) const {
    return sqrt(expr_(args...));
  }

  template<typename Variable>
  friend constexpr auto derive(Sqrt const& self, Variable v) {
    return derive(self.expr_, v) / (2 * sqrt(self.expr_));
  }

  friend std::ostream& operator<<(std::ostream& os, Sqrt const& self) {
    return os << "sqrt(" << self.expr_ << ')';
  }
};

template<typename Expr>
constexpr auto sqrt(Expr const& expr)
    -> std::enable_if_t<!IsZero<Expr>() && !IsPlusOne<Expr>(), Sqrt<Expr>> {
  return {expr};
}

template<typename T>
constexpr Zero<T> sqrt(Zero<T> zero) {
  return zero;
}

template<typename T>
constexpr PlusOne<T> sqrt(PlusOne<T>) {
  return PlusOne<T>{};
}


} // namespace auto_derive

#endif
