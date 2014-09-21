#ifndef OPERATORS_COS_HPP_
#define OPERATORS_COS_HPP_

#include <cmath>
#include "../../variable.hpp"

namespace auto_derive {

template<typename Expr>
class Cos : public Expression {
  const Expr expr_;

 public:
  constexpr Cos(Expr expr) : expr_(expr) {}

  constexpr Expr expr() const { return expr_; }

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(std::cos(expr_(context))) {
    return std::cos(expr_(context));
  }
};

template <typename T>
constexpr Cos<T> cos(T t) {
  return Cos<T>(t);
}

}

#include "./sin.hpp"
#include "../basic/unary_minus.hpp"

namespace auto_derive {

template <typename Expr, typename VarT, const char *var_name>
constexpr auto operator%(Cos<Expr> cos, Variable<VarT, var_name> v)
    -> decltype(-sin(cos.expr()) * (cos.expr() % v)) {
  return -sin(cos.expr()) * (cos.expr() % v);
}

}

#endif
