#ifndef OPERATORS_SIN_HPP_
#define OPERATORS_SIN_HPP_

#include <cmath>
#include "../../variable.hpp"

namespace auto_derive {

template<typename Expr>
class Sin : public Expression {
  const Expr expr_;

 public:
  constexpr Sin(Expr expr) : expr_(expr) {}

  constexpr const Expr& expr() const { return expr_; }

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
constexpr auto operator%(Sin<Expr> sin, Variable<VarT, var_name> v) {
  return cos(sin.expr()) * (sin.expr() % v);
}

}

#endif
