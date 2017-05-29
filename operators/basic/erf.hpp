#ifndef OPERATORS_BASIC_ERF_HPP_
#define OPERATORS_BASIC_ERF_HPP_

#include "../unary_operator.hpp"
#include <cmath>
#ifndef M_PI
#define M_PI std::acos(-1.)
#endif
namespace auto_derive {

template<typename Expr>
class Erf : public UnaryOperator<Expr> {
  __AUTO_DERIVE_USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  constexpr auto operator()(Args... args) const {
    return erf(expr_(args...));
  }

  template<typename Variable>
  friend constexpr auto derive(Erf const& self, Variable v) {
    // erf(x)' = exp(-x^2)*2/PI
    return derive(self.expr_, v) * exp(-1.0*self.expr_*self.expr_)*2/std::sqrt(M_PI);
  }

  friend std::ostream& operator<<(std::ostream& os, Erf const& self) {
    return os << "erf(" << self.expr_ << ')';
  }
};

template<typename Expr>
constexpr std::enable_if_t<!IsPlusOne<Expr>(), Erf<Expr>> erf(Expr const& expr) {
  return {expr};
}

template<typename T>
constexpr Zero<T> erf(PlusOne<T> expr) {
  return {expr};
}

} // namespace auto_derive

#endif
