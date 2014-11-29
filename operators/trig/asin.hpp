#ifndef OPERATORS_TRIG_ASIN_HPP_
#define OPERATORS_TRIG_ASIN_HPP_

#include <cmath>
#include "../basic/divide.hpp"
#include "../basic/square.hpp"
#include "../basic/subtract.hpp"
#include "../basic/unary_minus.hpp"
#include "../unary_operator.hpp"

namespace auto_derive {

template<typename Expr>
class Asin : public UnaryOperator<Expr> {
  __AUTO_DERIVE_USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  auto operator()(Args... args) const {
    return asin(expr_(args...));
  }

  template <typename Variable>
  friend constexpr auto derive(Asin const& self, Variable v) {
    return derive(self.expr(), v) / sqrt(1 - square(self.expr()));
  }

  friend std::ostream& operator<<(std::ostream& os, Asin const& self) {
    return os << "asin(" << self.expr_ << ')';
  }
};

template <typename T>
constexpr std::enable_if_t<!IsZero<T>(), Asin<T>> asin(T t) {
  return Asin<T>(t);
}

template <typename T>
constexpr Zero<T> asin(Zero<T> t) {
  return Zero<T>{};
}

}

#endif
