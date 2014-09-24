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

  template<typename... Args>
  constexpr auto operator()(Args&&... args) const {
    return lhs_(args...) - rhs_(args...);
  }

  template<typename VarT, const char *var_name>
  constexpr auto operator%(Variable<VarT, var_name> v) const {
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

  template<typename... Args>
  constexpr auto operator()(Args&&... args) const {
    return lhs_(args...) - rhs_;
  }

  template<typename VarT, const char *var_name>
  constexpr auto operator%(Variable<VarT, var_name> v) const {
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

  template<typename... Args>
  constexpr auto operator()(Args&&... args) const {
    return lhs_ - rhs_(args...);
  }

  template<typename VarT, const char *var_name>
  constexpr auto operator%(Variable<VarT, var_name> v) const {
    return -(rhs_ % v);
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator-(Lhs lhs, Rhs rhs)
    -> enable_if_t<
        (IsExpression<Lhs>() && !IsZero<Rhs>())
        || (IsExpression<Rhs>() && !IsZero<Lhs>()),
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
