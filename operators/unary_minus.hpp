#ifndef OPERATORS_UNARY_MINUS_H_
#define OPERATORS_UNARY_MINUS_H_

#include <map>
#include <string>
#include <type_traits>
#include "../expression.hpp"

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
  constexpr auto gradient() const -> decltype(-expr_.template gradient<T, str>()) {
    return -expr_.template gradient<T, str>();
  }
};

template<typename Expr>
constexpr auto operator-(Expr expr) ->
    typename std::enable_if<
      std::is_base_of<Expression, Expr>::value, UnaryMinus<Expr>
    >::type {
  return {expr};
}

template<typename Expr>
constexpr auto operator-(Expr expr) ->
    typename std::enable_if<
      std::is_base_of<ZeroType, Expr>::value, Expr
    >::type {
  return expr;
}

#endif
