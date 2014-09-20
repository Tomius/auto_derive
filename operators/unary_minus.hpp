#ifndef OPERATORS_UNARY_MINUS_H_
#define OPERATORS_UNARY_MINUS_H_

#include "../variable.hpp"

template<typename Expr>
class UnaryMinus : public Expression {
  Expr expr_;

 public:
  constexpr UnaryMinus(Expr expr) : expr_(expr) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(-expr_(context)) {
    return -expr_(context);
  }

  template<typename T, const char *str>
  constexpr auto gradient() const
      -> decltype(-expr_.template gradient<T, str>()) {
    return -expr_.template gradient<T, str>();
  }
};

template<typename Expr>
constexpr auto operator-(Expr expr)
    -> enable_if_t<IsExpression<Expr>::value, UnaryMinus<Expr>> {
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

#endif
