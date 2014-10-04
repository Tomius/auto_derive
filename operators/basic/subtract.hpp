#ifndef OPERATORS_SUBTRACT_HPP_
#define OPERATORS_SUBTRACT_HPP_

#include "../../variable.hpp"
#include "../binary_operator.hpp"
#include "./unary_minus.hpp"

namespace auto_derive {

template<typename Lhs, typename Rhs>
class Subtract : public BinaryOperator<Lhs, Rhs> {
  USING_BINARY_OPERATOR(Lhs, Rhs);

  template<typename... Args>
  constexpr auto operator()(Args&&... args) const {
    return lhs_(args...) - rhs_(args...);
  }

  template<typename VarT, const char *var_name>
  friend constexpr auto gradient(Subtract self, Variable<VarT, var_name> v) {
    return gradient(self.lhs_, v) - gradient(self.rhs_, v);
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator-(Lhs lhs, Rhs rhs)
    -> std::enable_if_t<
        (IsFunction<Lhs>() && !IsZero<Rhs>())
        || (IsFunction<Rhs>() && !IsZero<Lhs>()),
    Subtract<Lhs, Rhs>> {
  return {lhs, rhs};
}

template<typename Lhs, typename T>
constexpr Lhs operator-(Lhs lhs, Zero<T> rhs) {
  return lhs;
}

template<typename T, typename Rhs>
constexpr auto operator-(Zero<T> lhs, Rhs rhs) -> decltype(-rhs) {
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
