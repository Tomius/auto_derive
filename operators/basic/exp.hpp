#ifndef OPERATORS_BASIC_EXP_HPP_
#define OPERATORS_BASIC_EXP_HPP_

#include "./multiply.hpp"
#include "../unary_operator.hpp"

namespace auto_derive {

template<typename Expr>
class Exp : public UnaryOperator<Expr> {
  __AUTO_DERIVE_USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  constexpr auto operator()(Args... args) const {
    return exp(expr_(args...));
  }

  template<typename Variable>
  friend constexpr auto derive(Exp const& self, Variable v) {
    return self * derive(self.expr_, v);
  }

  friend std::ostream& operator<<(std::ostream& os, Exp const& self) {
    return os << "exp(" << self.expr_ << ')';
  }
};

template<typename Expr>
constexpr auto exp(Expr const& expr)
    -> std::enable_if_t<!IsPlusOne<Expr>(), Exp<Expr>> {
  return {expr};
}

template<typename T>
constexpr PlusOne<T> exp(Zero<T> zero) {
  return PlusOne<T>{};
}


} // namespace auto_derive

#endif
