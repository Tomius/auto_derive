#ifndef OPERATORS_UNARY_MINUS_HPP_
#define OPERATORS_UNARY_MINUS_HPP_

#include "../../variable.hpp"
#include "../unary_operator.hpp"

namespace auto_derive {

template<typename Expr>
class UnaryMinus : public UnaryOperator<Expr> {
  USING_UNARY_OPERATOR(Expr); 

  template<typename... Args>
  constexpr auto operator()(Args&&... args) const {
    return -expr_(args...);
  }

  template<typename T, const char *str>
  friend constexpr auto gradient(UnaryMinus self, Variable<T, str> v) {
    return -gradient(self.expr_, v);
  }
};

template<typename Expr>
constexpr auto operator-(Expr expr)
    -> std::enable_if_t<IsExpression<Expr>(), UnaryMinus<Expr>> {
  return {expr};
}

template<typename T>
constexpr Zero<T> operator-(Zero<T> zero) {
  return zero;
}

template<typename T>
constexpr MinusOne<T> operator-(PlusOne<T>) {
  return MinusOne<T>{};
}

template<typename T>
constexpr PlusOne<T> operator-(MinusOne<T>) {
  return PlusOne<T>{};
}

} // namespace auto_derive

#endif
