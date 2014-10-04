#ifndef OPERATORS_TRIG_COS_HPP_
#define OPERATORS_TRIG_COS_HPP_

#include <cmath>
#include "../../variable.hpp"
#include "../unary_operator.hpp"

namespace auto_derive {

template<typename Expr>
class Cos : public UnaryOperator<Expr> {
  USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  auto operator()(Args&&... args) const {
    return std::cos(expr_(args...));
  }
};

template <typename T>
constexpr std::enable_if_t<!IsZero<T>(), Cos<T>> cos(T t) {
  return Cos<T>(t);
}

template <typename T>
constexpr PlusOne<T> cos(Zero<T> t) {
  return PlusOne<T>{};
}

}

#include "./sin.hpp"
#include "../basic/unary_minus.hpp"

namespace auto_derive {

template <typename Expr, typename VarT, const char *var_name>
constexpr auto gradient(Cos<Expr> cos, Variable<VarT, var_name> v) {
  return -sin(cos.expr()) * gradient(cos.expr(), v);
}

}

#endif
