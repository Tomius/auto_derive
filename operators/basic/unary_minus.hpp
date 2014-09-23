#ifndef OPERATORS_UNARY_MINUS_HPP_
#define OPERATORS_UNARY_MINUS_HPP_

#include "../../variable.hpp"

namespace auto_derive {

template<typename Expr>
class UnaryMinus : public Expression {
  Expr expr_;

 public:
  constexpr UnaryMinus(Expr expr) : expr_(expr) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const {
    return -expr_(context);
  }

  template<typename T, const char *str>
  constexpr auto operator%(Variable<T, str> v) const {
    return -(expr_ % v);
  }
};

template<typename Expr>
constexpr auto operator-(Expr expr)
    -> enable_if_t<IsExpression<Expr>(), UnaryMinus<Expr>> {
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