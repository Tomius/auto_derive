#ifndef OPERATORS_BASIC_UNARY_MINUS_HPP_
#define OPERATORS_BASIC_UNARY_MINUS_HPP_

#include "../unary_operator.hpp"

namespace auto_derive {

template<typename Expr>
class UnaryMinus : public UnaryOperator<Expr> {
  __AUTO_DERIVE_USING_UNARY_OPERATOR(Expr);

  template<typename... Args>
  constexpr auto operator()(Args... args) const {
    return -expr_(args...);
  }

  template<typename Variable>
  friend constexpr auto derive(UnaryMinus const& self, Variable v) {
    return -derive(self.expr_, v);
  }

  enum { precendence = 2 };

  friend std::ostream& operator<<(std::ostream& os, UnaryMinus const& self) {
    os << set_precendence(precendence) << '-' << self.expr_;
    return os;
  }
};

template<typename Expr>
constexpr auto operator-(Expr const& expr)
    -> std::enable_if_t<IsFunction<Expr>(), UnaryMinus<Expr>> {
  return {expr};
}

template<typename T>
constexpr Zero<T> operator-(Zero<T> zero) {
  return zero;
}

template<typename T>
constexpr MinusOne<T> operator-(PlusOne<T>) {
  return MinusOne<T>{};
}

template<typename T>
constexpr PlusOne<T> operator-(MinusOne<T>) {
  return PlusOne<T>{};
}

} // namespace auto_derive

#endif
