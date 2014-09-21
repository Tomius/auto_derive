#ifndef OPERATORS_MULTIPLY_HPP_
#define OPERATORS_MULTIPLY_HPP_

#include "../../variable.hpp"
#include "./add.hpp"

template<typename Lhs, typename Rhs, typename Enable = void>
class Multiply;

template<typename Lhs, typename Rhs>
class Multiply<Lhs, Rhs, enable_if_t<IsExpression<Lhs>() && IsExpression<Rhs>()>>
    : public Expression {
  const Lhs lhs_;
  const Rhs rhs_;

 public:
  constexpr Multiply(Lhs lhs, Rhs rhs) : lhs_(lhs), rhs_(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_(context) * rhs_(context)) {
    return lhs_(context) * rhs_(context);
  }

  template<typename T, const char *str>
  constexpr auto gradient(Variable<T, str> v) const
      -> decltype(lhs_ * rhs_.gradient(v) +
                  rhs_ * lhs_.gradient(v)) {
    return lhs_ * rhs_.gradient(v) +
    	     rhs_ * lhs_.gradient(v);
  }
};

template<typename Lhs, typename Constant>
class Multiply<Lhs, Constant,
    enable_if_t<IsExpression<Lhs>() && !IsExpression<Constant>()>>
    : public Expression {

  const Lhs lhs_;
  const Constant rhs_;

 public:
  constexpr Multiply(Lhs lhs, Constant rhs) : lhs_(lhs), rhs_(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_(context) * rhs_) {
    return lhs_(context) * rhs_;
  }

  template<typename T, const char *str>
  constexpr auto gradient(Variable<T, str> v) const
      -> decltype(lhs_.gradient(v) * rhs_) {
    return lhs_.gradient(v) * rhs_;
  }
};

template<typename Constant, typename Rhs>
class Multiply<Constant, Rhs,
    enable_if_t<!IsExpression<Constant>() && IsExpression<Rhs>()>>
    : public Expression {

  const Constant lhs_;
  const Rhs rhs_;

 public:
  constexpr Multiply(Constant lhs, Rhs rhs) : lhs_(lhs), rhs_(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_ * rhs_(context)) {
    return lhs_ * rhs_(context);
  }

  template<typename T, const char *str>
  constexpr auto gradient(Variable<T, str> v) const
      -> decltype(lhs_ * rhs_.gradient(v)) {
    return lhs_ * rhs_.gradient(v);
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator*(Lhs lhs, Rhs rhs)
    -> enable_if_t<
        (IsExpression<Lhs>() && IsExpression<Rhs>())
        || (IsExpression<Lhs>() && !IsExpression<Rhs>() &&
            !IsOne<Rhs>() && !IsZero<Rhs>())
        || (!IsExpression<Lhs>() && IsExpression<Rhs>() &&
            !IsOne<Lhs>() && !IsZero<Lhs>()),
    Multiply<Lhs, Rhs, void>> {
  return {lhs, rhs};
}

template<typename Lhs, typename T>
constexpr auto operator*(Lhs lhs, PlusOne<T> rhs)
    -> enable_if_t<!IsZero<Lhs>(), Lhs> {
  return lhs;
}

template<typename T, typename Rhs>
constexpr auto operator*(PlusOne<T> lhs, Rhs rhs)
    -> enable_if_t<!IsZero<Rhs>(), Rhs> {
  return rhs;
}

template<typename T, typename U>
constexpr PlusOne<decltype(T{1}*U{1})> operator*(PlusOne<T> lhs, PlusOne<U> rhs) {
  return PlusOne<decltype(T{1}*U{1})>{};
}

template<typename Lhs, typename T>
constexpr auto operator*(Lhs lhs, MinusOne<T> rhs)
    -> enable_if_t<!IsZero<Lhs>(), decltype(-lhs)> {
  return -lhs;
}

template<typename T, typename Rhs>
constexpr auto operator*(MinusOne<T> lhs, Rhs rhs)
    -> enable_if_t<!IsZero<Rhs>(), decltype(-rhs)> {
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

#endif
