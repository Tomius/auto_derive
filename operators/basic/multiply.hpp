#ifndef OPERATORS_BASIC_MULTIPLY_HPP_
#define OPERATORS_BASIC_MULTIPLY_HPP_

#include "./add.hpp"
#include "../binary_operator.hpp"

namespace auto_derive {

template<typename Lhs, typename Rhs>
class Multiply : public BinaryOperator<Lhs, Rhs> {
  __AUTO_DERIVE_USING_BINARY_OPERATOR(Lhs, Rhs);

  template<typename... Args>
  constexpr auto operator()(Args... args) const {
    return lhs_(args...) * rhs_(args...);
  }

  template<typename Variable>
  friend constexpr auto derive(Multiply const& self, Variable v) {
    return self.lhs_*derive(self.rhs_, v) + self.rhs_*derive(self.lhs_, v);
  }

  enum { precendence = 3 };

  friend std::ostream& operator<<(std::ostream& os, Multiply const& self) {
    if (!IsFunction<Rhs>()) {
      // f(x)*c should be printed as c*f(x) by convention
      os << set_precendence(precendence) << self.rhs_ << '*' << self.lhs_;
    } else {
      os << set_precendence(precendence) << self.lhs_ << '*' << self.rhs_;
    }
    return os;
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator*(Lhs const& lhs, Rhs const& rhs)
    -> std::enable_if_t<
        (IsFunction<Lhs>() && !IsOne<Rhs>() && !IsZero<Rhs>())
        || (IsFunction<Rhs>() && !IsOne<Lhs>() && !IsZero<Lhs>()),
    Multiply<Lhs, Rhs>> {
  return {lhs, rhs};
}

template<typename Lhs, typename T>
constexpr auto operator*(Lhs const& lhs, PlusOne<T> rhs)
    -> std::enable_if_t<!IsZero<Lhs>(), Lhs> {
  return lhs;
}

template<typename T, typename Rhs>
constexpr auto operator*(PlusOne<T> lhs, Rhs const& rhs)
    -> std::enable_if_t<!IsZero<Rhs>(), Rhs> {
  return rhs;
}

template<typename T, typename U>
constexpr auto operator*(PlusOne<T> lhs, PlusOne<U> rhs) {
  return PlusOne<decltype(lhs.value*rhs.value)>{};
}

template<typename Lhs, typename T>
constexpr auto operator*(Lhs const& lhs, MinusOne<T> rhs)
    -> std::enable_if_t<!IsZero<Lhs>(), decltype(-lhs)> {
  return -lhs;
}

template<typename T, typename Rhs>
constexpr auto operator*(MinusOne<T> lhs, Rhs const& rhs)
    -> std::enable_if_t<!IsZero<Rhs>(), decltype(-rhs)> {
  return -rhs;
}

template<typename T, typename U>
constexpr auto operator*(MinusOne<T> lhs, MinusOne<U> rhs) {
  return PlusOne<decltype(lhs.value*rhs.value)>{};
}

template<typename T, typename U>
constexpr auto operator*(PlusOne<T> lhs, MinusOne<U> rhs) {
  return MinusOne<decltype(lhs.value*rhs.value)>{};
}

template<typename T, typename U>
constexpr auto operator*(MinusOne<T> lhs, PlusOne<U> rhs) {
  return MinusOne<decltype(lhs.value*rhs.value)>{};
}

template<typename Lhs, typename T>
constexpr Zero<T> operator*(Lhs const& lhs, Zero<T> rhs) {
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
constexpr Zero<T> operator*(Zero<T> lhs, Rhs const& rhs) {
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
constexpr auto operator*(Zero<T> lhs, Zero<U> rhs) {
  return Zero<decltype(lhs.value*rhs.value)>{};
}

} // namespace auto_derive

#endif
