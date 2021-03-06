#ifndef OPERATORS_TRIG_COS_HPP_
#define OPERATORS_TRIG_COS_HPP_

#include "../unary_operator.hpp"

namespace auto_derive {

template<typename Expr>
class Cos : public UnaryOperator<Expr> {
  __AUTO_DERIVE_USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  constexpr auto operator()(Args... args) const {
    return cos(expr_(args...));
  }

  friend std::ostream& operator<<(std::ostream& os, Cos const& self) {
    return os << "cos(" << self.expr_ << ')';
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
#include "../basic/multiply.hpp"
#include "../basic/unary_minus.hpp"

namespace auto_derive {

template <typename Expr, typename Variable>
constexpr auto derive(Cos<Expr> const& cos, Variable v) {
  return -sin(cos.expr()) * derive(cos.expr(), v);
}

}

#endif
