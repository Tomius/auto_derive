#ifndef OPERATORS_ADD_HPP_
#define OPERATORS_ADD_HPP_

#include "../../variable.hpp"
#include "../binary_operator.hpp"

namespace auto_derive {

template<typename Lhs, typename Rhs>
class Add : public BinaryOperator<Lhs, Rhs> {
  USING_BINARY_OPERATOR(Lhs, Rhs);

  template<typename... Args>
  constexpr auto operator()(Args&&... args) const {
    return lhs_(args...) + rhs_(args...);
  }

  template<typename T, const char *str>
  friend constexpr auto gradient(Add self, Variable<T, str> v) {
    return gradient(self.lhs_, v) + gradient(self.rhs_, v);
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator+(Lhs lhs, Rhs rhs)
    -> std::enable_if_t<(
      IsFunction<Lhs>() && !IsZero<Rhs>())
      || (IsFunction<Rhs>() && !IsZero<Lhs>()),
    Add<Lhs, Rhs>> {
  return {lhs, rhs};
}

template<typename Lhs, typename T>
constexpr Lhs operator+(Lhs lhs, Zero<T> rhs) {
  return lhs;
}

template<typename T, typename Rhs>
constexpr Rhs operator+(Zero<T> lhs, Rhs rhs) {
  return rhs;
}

template<typename T, typename U>
constexpr Zero<decltype(T{0}+U{0})> operator+(Zero<T> lhs, Zero<U> rhs) {
  return Zero<decltype(T{0}+U{0})>{};
}

template<typename T, typename U>
constexpr Zero<decltype(T{1}+U{-1})> operator+(PlusOne<T> lhs, MinusOne<U> rhs) {
  return Zero<decltype(T{1}+U{-1})>{};
}

template<typename T, typename U>
constexpr Zero<decltype(T{-1}+U{1})> operator+(MinusOne<T> lhs, PlusOne<U> rhs) {
  return Zero<decltype(T{-1}+U{1})>{};
}

} // namespace auto_derive

#endif
