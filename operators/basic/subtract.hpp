#ifndef OPERATORS_SUBTRACT_HPP_
#define OPERATORS_SUBTRACT_HPP_

#include "../../variable.hpp"
#include "./unary_minus.hpp"

namespace auto_derive {

template<typename Lhs, typename Rhs, typename Enable = void>
class Subtract;

template<typename Lhs, typename Rhs>
class Subtract<Lhs, Rhs, enable_if_t<IsExpression<Lhs>() && IsExpression<Rhs>()>>
    : public Expression {
  const Lhs lhs_;
  const Rhs rhs_;

 public:
  constexpr Subtract(Lhs lhs, Rhs rhs) : lhs_(lhs), rhs_(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_(context) - rhs_(context)) {
    return lhs_(context) - rhs_(context);
  }

  template<typename T, const char *str>
  constexpr auto operator%(Variable<T, str> v) const
      -> decltype((lhs_ % v) - (rhs_ % v)) {
    return (lhs_ % v) - (rhs_ % v);
  }
};

template<typename Lhs, typename Constant>
class Subtract<Lhs, Constant,
    enable_if_t<IsExpression<Lhs>() && !IsExpression<Constant>()>>
    : public Expression {

  const Lhs lhs_;
  const Constant rhs_;

 public:
  constexpr Subtract(Lhs lhs, Constant rhs) : lhs_(lhs), rhs_(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_(context) - rhs_) {
    return lhs_(context) - rhs_;
  }

  template<typename T, const char *str>
  constexpr auto operator%(Variable<T, str> v) const -> decltype(lhs_ % v) {
    return lhs_ % v;
  }
};

template<typename Constant, typename Rhs>
class Subtract<Constant, Rhs,
    enable_if_t<!IsExpression<Constant>() && IsExpression<Rhs>()>>
    : public Expression {

  const Constant lhs_;
  const Rhs rhs_;

 public:
  constexpr Subtract(Constant lhs, Rhs rhs) : lhs_(lhs), rhs_(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_ - rhs_(context)) {
    return lhs_ - rhs_(context);
  }

  template<typename T, const char *str>
  constexpr auto operator%(Variable<T, str> v) const -> decltype(-(rhs_ % v)) {
    return -(rhs_ % v);
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator-(Lhs lhs, Rhs rhs)
    -> enable_if_t<
        (IsExpression<Lhs>() && IsExpression<Rhs>())
        || (IsExpression<Lhs>() && !IsExpression<Rhs>() && !IsZero<Rhs>())
        || (!IsExpression<Lhs>() && IsExpression<Rhs>() && !IsZero<Lhs>()),
    Subtract<Lhs, Rhs, void>> {
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
