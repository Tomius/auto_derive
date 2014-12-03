#ifndef OPERATORS_BASIC_SUBTRACT_HPP_
#define OPERATORS_BASIC_SUBTRACT_HPP_

#include "./unary_minus.hpp"
#include "../binary_operator.hpp"

namespace auto_derive {

template<typename Lhs, typename Rhs>
class Subtract : public BinaryOperator<Lhs, Rhs> {
  __AUTO_DERIVE_USING_BINARY_OPERATOR(Lhs, Rhs);

  template<typename... Args>
  constexpr auto operator()(Args... args) const {
    return lhs_(args...) - rhs_(args...);
  }

  template<typename Variable>
  friend constexpr auto derive(Subtract const& self, Variable v) {
    return derive(self.lhs_, v) - derive(self.rhs_, v);
  }

  enum { precendence = 4 };

  friend std::ostream& operator<<(std::ostream& os, Subtract const& self) {
    os << set_precendence(precendence) << self.lhs_ << '-';
    os << set_precendence(precendence - 1) << self.rhs_;
    return os;
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator-(Lhs const& lhs, Rhs const& rhs)
    -> std::enable_if_t<
        (IsFunction<Lhs>() && !IsZero<Rhs>())
        || (IsFunction<Rhs>() && !IsZero<Lhs>()),
    Subtract<Lhs, Rhs>> {
  return {lhs, rhs};
}

template<typename Lhs, typename T>
constexpr Lhs operator-(Lhs const& lhs, Zero<T> rhs) {
  return lhs;
}

template<typename T, typename Rhs>
constexpr auto operator-(Zero<T> lhs, Rhs const& rhs) -> decltype(-rhs) {
  return -rhs;
}

template<typename T, typename U>
constexpr Zero<decltype(T{0}-U{0})> operator-(Zero<T> lhs, Zero<U> rhs) {
  return Zero<decltype(T{0}-U{0})>{};
}

template<typename T, typename U>
constexpr Zero<decltype(T{1}-U{1})> operator-(PlusOne<T> lhs, PlusOne<U> rhs) {
  return Zero<decltype(T{1}-U{1})>{};
}

template<typename T, typename U>
constexpr Zero<decltype(T{-1}-U{-1})> operator-(MinusOne<T> lhs, MinusOne<U> rhs) {
  return Zero<decltype(T{-1}-U{-1})>{};
}

} // namespace auto_derive

#endif
