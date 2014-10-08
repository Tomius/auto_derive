#ifndef OPERATORS_BASIC_LOG_HPP_
#define OPERATORS_BASIC_LOG_HPP_

#include "../../variable.hpp"
#include "../unary_operator.hpp"
#include "./divide.hpp"

namespace auto_derive {

template<typename Expr>
class Log : public UnaryOperator<Expr> {
  USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  constexpr auto operator()(Args&&... args) const {
    return std::log(expr_(std::forward<Args>(args)...));
  }

  template<typename Variable>
  friend constexpr auto gradient(Log const& self, Variable v) {
    // ln(x)' = 1/x * x' = x' / x
    return gradient(self.expr_, v) / self.expr_;
  }

  friend std::ostream& operator<<(std::ostream& os, Log const& self) {
    return os << "log(" << self.expr_ << ')';
  }
};

template<typename Expr>
constexpr std::enable_if_t<!IsPlusOne<Expr>(), Log<Expr>> log(Expr expr) {
  return {expr};
}

template<typename T>
constexpr Zero<T> log(PlusOne<T> expr) {
  return {expr};
}

} // namespace auto_derive

#endif