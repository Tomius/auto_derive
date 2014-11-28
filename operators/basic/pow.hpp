#ifndef OPERATORS_BASIC_POW_HPP_
#define OPERATORS_BASIC_POW_HPP_

#include "./log.hpp"
#include "./divide.hpp"
#include "../binary_operator.hpp"

namespace auto_derive {

template<typename Lhs, typename Rhs>
class Pow : public BinaryOperator<Lhs, Rhs> {
  USING_BINARY_OPERATOR(Lhs, Rhs);

  template<typename... Args>
  auto operator()(Args... args) const {
    return pow(lhs_(args...), rhs_(args...));
  }

  // (f^g)' = f^g * g' * ln(f) + f^(g-1) * g * f'
  // See http://mathforum.org/library/drmath/view/53679.html - equation [10]
  template<typename Variable>
  friend constexpr auto derive(Pow const& self, Variable v) {
    auto const& f = self.lhs_; auto const& g = self.rhs_;
    return pow(f, g) * derive(g, v) * log(f) + pow(f, g-1) * g * derive(f, v);
  }

  friend std::ostream& operator<<(std::ostream& os, Pow const& self) {
    return os << "pow(" << self.lhs_ << ", " << self.rhs_ << ')';
  }
};

template<typename Lhs, typename Rhs>
constexpr auto pow(Lhs const& lhs, Rhs const& rhs)
    -> std::enable_if_t<(
      IsFunction<Lhs>() && !IsZero<Rhs>() && !IsOne<Rhs>())
      || (IsFunction<Rhs>() && !IsZero<Lhs>() && !IsPlusOne<Lhs>()),
    Pow<Lhs, Rhs>> {
  return {lhs, rhs};
}

template<typename Lhs, typename T>
constexpr PlusOne<T> pow(Lhs const& lhs, Zero<T> rhs) {
  return PlusOne<T>{};
}

template<typename T, typename Rhs>
constexpr std::enable_if_t<!IsZero<Rhs>(), Zero<T>> pow(Zero<T> lhs, Rhs const& rhs) {
  return Zero<T>{};
}

template<typename Lhs, typename T>
constexpr std::enable_if_t<!IsZero<Lhs>(), Lhs> pow(Lhs const& lhs, PlusOne<T> rhs) {
  return lhs;
}

template<typename Lhs, typename T>
constexpr auto pow(Lhs const& lhs, MinusOne<T> rhs)
    -> std::enable_if_t<!IsZero<Lhs>(), decltype(PlusOne<T>{} / lhs)> {
  return PlusOne<T>{} / lhs;
}

template<typename T, typename Rhs>
constexpr auto pow(PlusOne<T> lhs, Rhs const& rhs)
    -> std::enable_if_t<!IsZero<Rhs>() && !IsOne<Rhs>(), PlusOne<T>> {
  return lhs;
}

} // namespace auto_derive

#endif
