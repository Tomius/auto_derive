#ifndef OPERATORS_SIN_HPP_
#define OPERATORS_SIN_HPP_

#include <cmath>
#include "../../variable.hpp"
#include "../unary_operator.hpp"

namespace auto_derive {

template<typename Expr>
class Sin : public UnaryOperator<Expr> {
  USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  auto operator()(Args&&... args) const {
    return std::sin(expr_(args...));
  }
};

template <typename T>
constexpr Sin<T> sin(T t) {
  return Sin<T>(t);
}

}

#include "./cos.hpp"

namespace auto_derive {

template <typename Expr, typename VarT, const char *var_name>
constexpr auto gradient(Sin<Expr> sin, Variable<VarT, var_name> v) {
  return cos(sin.expr()) * gradient(sin.expr(), v);
}

}

#endif
