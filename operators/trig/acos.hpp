#ifndef OPERATORS_TRIG_ACOS_HPP_
#define OPERATORS_TRIG_ACOS_HPP_

#include "../basic/divide.hpp"
#include "../basic/square.hpp"
#include "../basic/subtract.hpp"
#include "../basic/unary_minus.hpp"
#include "../unary_operator.hpp"

namespace auto_derive {

template<typename Expr>
class Acos : public UnaryOperator<Expr> {
  __AUTO_DERIVE_USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  constexpr auto operator()(Args... args) const {
    return acos(expr_(args...));
  }

  template <typename Variable>
  friend constexpr auto derive(Acos const& self, Variable v) {
    return -derive(self.expr(), v) / sqrt(1 - square(self.expr()));
  }

  friend std::ostream& operator<<(std::ostream& os, Acos const& self) {
    return os << "acos(" << self.expr_ << ')';
  }
};

template <typename T>
constexpr std::enable_if_t<!IsPlusOne<T>(), Acos<T>> acos(T t) {
  return Acos<T>(t);
}

template <typename T>
constexpr Zero<T> acos(PlusOne<T> t) {
  return Zero<T>{};
}

}

#endif
