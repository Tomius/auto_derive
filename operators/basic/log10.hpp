#ifndef OPERATORS_BASIC_LOG10_HPP_
#define OPERATORS_BASIC_LOG10_HPP_

#include "../../variable.hpp"
#include "../unary_operator.hpp"
#include "./divide.hpp"

namespace auto_derive {

template<typename Expr>
class Log10 : public UnaryOperator<Expr> {
  USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  constexpr auto operator()(Args&&... args) const {
    return std::log10(expr_(std::forward<Args>(args)...));
  }

  template<typename Variable>
  friend constexpr auto gradient(Log10 const& self, Variable v) {
    // log10(x)' = ln(x)' / ln(10) = (x' / x) / ln(10)
    constexpr double ln10 = 2.30258509299;
    return gradient(self.expr_, v) / (self.expr_ * ln10);
  }

  friend std::ostream& operator<<(std::ostream& os, Log10 const& self) {
    return os << "log10(" << self.expr_ << ')';
  }
};

template<typename Expr>
constexpr std::enable_if_t<!IsPlusOne<Expr>(), Log<Expr>> log10(Expr const& expr) {
  return {expr};
}

template<typename T>
constexpr Zero<T> log10(PlusOne<T> expr) {
  return {expr};
}

} // namespace auto_derive

#endif
