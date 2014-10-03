#ifndef OPERATORS_DIVIDE_HPP_
#define OPERATORS_DIVIDE_HPP_

#include "../../variable.hpp"
#include "../binary_operator.hpp"
#include "./subtract.hpp"
#include "./multiply.hpp"
#include "./unary_minus.hpp"

namespace auto_derive {

template<typename Lhs, typename Rhs>
class Divide : public BinaryOperator<Lhs, Rhs> {
  USING_BINARY_OPERATOR(Lhs, Rhs);

  template<typename... Args>
  constexpr auto operator()(Args&&... args) const {
    return lhs_(args...) / rhs_(args...);
  }

  template<typename VarT, const char *var_name>
  friend constexpr auto gradient(Divide self, Variable<VarT, var_name> v) {
    return (gradient(self.lhs_, v)*self.rhs_ - gradient(self.rhs_, v)*self.lhs_)
           / (self.rhs_*self.rhs_);
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator/(Lhs lhs, Rhs rhs)
    -> std::enable_if_t<
      (IsExpression<Lhs>() && !IsOne<Rhs>() && !IsZero<Rhs>())
      || (IsExpression<Rhs>() && !IsZero<Lhs>()),
    Divide<Lhs, Rhs>> {
  return {lhs, rhs};
}

template<typename Lhs, typename T>
constexpr auto operator/(Lhs lhs, PlusOne<T> rhs)
    -> std::enable_if_t<!IsZero<Lhs>(), Lhs> {
  return lhs;
}

template<typename Lhs, typename T>
constexpr auto operator/(Lhs lhs, MinusOne<T> rhs)
    -> std::enable_if_t<!IsZero<Lhs>(), decltype(-lhs)> {
  return -lhs;
}

template<typename T, typename U>
constexpr PlusOne<decltype(T{1}/U{1})> operator/(PlusOne<T> lhs, PlusOne<U> rhs) {
  return PlusOne<decltype(T{1}/U{1})>{};
}

template<typename T, typename U>
constexpr PlusOne<decltype(T{-1}/U{-1})> operator/(MinusOne<T> lhs, MinusOne<U> rhs) {
  return PlusOne<decltype(T{-1}/U{-1})>{};
}

template<typename T, typename U>
constexpr MinusOne<decltype(T{1}/U{-1})> operator/(PlusOne<T> lhs, MinusOne<U> rhs) {
  return MinusOne<decltype(T{1}/U{-1})>{};
}

template<typename T, typename U>
constexpr MinusOne<decltype(T{-1}/U{1})> operator/(MinusOne<T> lhs, PlusOne<U> rhs) {
  return MinusOne<decltype(T{-1}/U{1})>{};
}

template<typename Lhs, typename T>
constexpr NaN<T> operator/(Lhs lhs, Zero<T> rhs) {
  return NaN<T>{};
}

template<typename T, typename U>
constexpr Zero<U> operator/(PlusOne<T> lhs, Zero<U> rhs) {
  return NaN<T>{};
}

template<typename T, typename U>
constexpr Zero<U> operator/(MinusOne<T> lhs, Zero<U> rhs) {
  return NaN<T>{};
}

template<typename T, typename Rhs>
constexpr Zero<T> operator/(Zero<T> lhs, Rhs rhs) {
  return lhs;
}

template<typename T, typename U>
constexpr Zero<T> operator/(Zero<T> lhs, PlusOne<U> rhs) {
  return lhs;
}

template<typename T, typename U>
constexpr Zero<T> operator/(Zero<T> lhs, MinusOne<U> rhs) {
  return lhs;
}

template<typename T, typename U>
constexpr NaN<decltype(T{0}/U{0})> operator/(Zero<T> lhs, Zero<U> rhs) {
  return NaN<decltype(T{0}/U{0})>{};
}

} // namespace auto_derive

#endif
