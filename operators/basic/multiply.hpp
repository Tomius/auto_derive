#ifndef OPERATORS_BASIC_MULTIPLY_HPP_
#define OPERATORS_BASIC_MULTIPLY_HPP_

#include "../../variable.hpp"
#include "../binary_operator.hpp"
#include "./add.hpp"

namespace auto_derive {

template<typename Lhs, typename Rhs>
class Multiply : public BinaryOperator<Lhs, Rhs> {
  USING_BINARY_OPERATOR(Lhs, Rhs);

  template<typename... Args>
  constexpr auto operator()(Args&&... args) const {
    return lhs_(std::forward<Args>(args)...) * rhs_(std::forward<Args>(args)...);
  }

  template<typename Variable>
  friend constexpr auto gradient(Multiply const& self, Variable v) {
    return self.lhs_*gradient(self.rhs_, v) + self.rhs_*gradient(self.lhs_, v);
  }

  static int precendence() { return 3; }

  friend std::ostream& operator<<(std::ostream& os, Multiply const& self) {
    os << put_parenthesis(precendence()) << self.lhs_ << " * " << self.rhs_;
    return os;
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator*(Lhs lhs, Rhs rhs)
    -> std::enable_if_t<
        (IsFunction<Lhs>() && !IsOne<Rhs>() && !IsZero<Rhs>())
        || (IsFunction<Rhs>() && !IsOne<Lhs>() && !IsZero<Lhs>()),
    Multiply<Lhs, Rhs>> {
  return {lhs, rhs};
}

template<typename Lhs, typename T>
constexpr auto operator*(Lhs lhs, PlusOne<T> rhs)
    -> std::enable_if_t<!IsZero<Lhs>(), Lhs> {
  return lhs;
}

template<typename T, typename Rhs>
constexpr auto operator*(PlusOne<T> lhs, Rhs rhs)
    -> std::enable_if_t<!IsZero<Rhs>(), Rhs> {
  return rhs;
}

template<typename T, typename U>
constexpr PlusOne<decltype(T{1}*U{1})> operator*(PlusOne<T> lhs, PlusOne<U> rhs) {
  return PlusOne<decltype(T{1}*U{1})>{};
}

template<typename Lhs, typename T>
constexpr auto operator*(Lhs lhs, MinusOne<T> rhs)
    -> std::enable_if_t<!IsZero<Lhs>(), decltype(-lhs)> {
  return -lhs;
}

template<typename T, typename Rhs>
constexpr auto operator*(MinusOne<T> lhs, Rhs rhs)
    -> std::enable_if_t<!IsZero<Rhs>(), decltype(-rhs)> {
  return -rhs;
}

template<typename T, typename U>
constexpr PlusOne<decltype(T{-1}*U{-1})> operator*(MinusOne<T> lhs, MinusOne<U> rhs) {
  return PlusOne<decltype(T{-1}*U{-1})>{};
}

template<typename T, typename U>
constexpr MinusOne<decltype(T{1}*U{-1})> operator*(PlusOne<T> lhs, MinusOne<U> rhs) {
  return MinusOne<decltype(T{1}*U{-1})>{};
}

template<typename T, typename U>
constexpr MinusOne<decltype(T{-1}*U{1})> operator*(MinusOne<T> lhs, PlusOne<U> rhs) {
  return MinusOne<decltype(T{-1}*U{1})>{};
}

template<typename Lhs, typename T>
constexpr Zero<T> operator*(Lhs lhs, Zero<T> rhs) {
  return rhs;
}

template<typename T, typename U>
constexpr Zero<U> operator*(PlusOne<T> lhs, Zero<U> rhs) {
  return rhs;
}

template<typename T, typename U>
constexpr Zero<U> operator*(MinusOne<T> lhs, Zero<U> rhs) {
  return rhs;
}

template<typename T, typename Rhs>
constexpr Zero<T> operator*(Zero<T> lhs, Rhs rhs) {
  return lhs;
}

template<typename T, typename U>
constexpr Zero<T> operator*(Zero<T> lhs, PlusOne<U> rhs) {
  return lhs;
}

template<typename T, typename U>
constexpr Zero<T> operator*(Zero<T> lhs, MinusOne<U> rhs) {
  return lhs;
}

template<typename T, typename U>
constexpr Zero<decltype(T{0}*U{0})> operator*(Zero<T> lhs, Zero<U> rhs) {
  return Zero<decltype(T{0}*U{0})>{};
}

} // namespace auto_derive

#endif
