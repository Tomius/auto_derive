#ifndef OPERATORS_TRIG_SINH_HPP_
#define OPERATORS_TRIG_SINH_HPP_

#include "../unary_operator.hpp"

namespace auto_derive {

template<typename Expr>
class Sinh : public UnaryOperator<Expr> {
  __AUTO_DERIVE_USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  constexpr auto operator()(Args... args) const {
    return sinh(expr_(args...));
  }

  friend std::ostream& operator<<(std::ostream& os, Sinh const& self) {
    return os << "sinh(" << self.expr_ << ')';
  }
};

template <typename T>
constexpr std::enable_if_t<!IsZero<T>(), Sinh<T>> sinh(T t) {
  return Sinh<T>(t);
}

template <typename T>
constexpr Zero<T> sinh(Zero<T> t) {
  return Zero<T>{};
}

}

#include "./cosh.hpp"
#include "../basic/multiply.hpp"

namespace auto_derive {

template <typename Expr, typename Variable>
constexpr auto derive(Sinh<Expr> const& self, Variable v) {
  return cosh(self.expr()) * derive(self.expr(), v);
}

}

#endif
