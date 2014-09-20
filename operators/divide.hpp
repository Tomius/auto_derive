#ifndef OPERATORS_DIVIDE_HPP_
#define OPERATORS_DIVIDE_HPP_

#include "../variable.hpp"
#include "./subtract.hpp"
#include "./multiply.hpp"
#include "./unary_minus.hpp"

template<typename Lhs, typename Rhs, typename Enable = void>
class Divide;

template<typename Lhs, typename Rhs>
class Divide<Lhs, Rhs,
    enable_if_t<IsExpression<Lhs>::value && IsExpression<Rhs>::value>>
    : public Expression {
  const Lhs lhs_;
  const Rhs rhs_;

 public:
  constexpr Divide(Lhs lhs, Rhs rhs) : lhs_(lhs), rhs_(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_(context) / rhs_(context)) {
    return lhs_(context) / rhs_(context);
  }

  template<typename T, const char *str>
  constexpr auto gradient() const
      -> decltype((lhs_.template gradient<T, str>() * rhs_ -
                   rhs_.template gradient<T, str>() * lhs_) / (rhs_*rhs_)) {
    return (lhs_.template gradient<T, str>() * rhs_ -
            rhs_.template gradient<T, str>() * lhs_) / (rhs_*rhs_);
  }
};

template<typename Lhs, typename Constant>
class Divide<Lhs, Constant,
    enable_if_t<IsExpression<Lhs>::value && !IsExpression<Constant>::value>>
    : public Expression {

  const Lhs lhs_;
  const Constant rhs_;

 public:
  constexpr Divide(Lhs lhs, Constant rhs) : lhs_(lhs), rhs_(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_(context) / rhs_) {
    return lhs_(context) / rhs_;
  }

  template<typename T, const char *str>
  constexpr auto gradient() const
      -> decltype(lhs_.template gradient<T, str>() / rhs_) {
    return lhs_.template gradient<T, str>() / rhs_;
  }
};

template<typename Constant, typename Rhs>
class Divide<Constant, Rhs,
    enable_if_t<!IsExpression<Constant>::value &&
                            IsExpression<Rhs>::value>>
    : public Expression {

  const Constant lhs_;
  const Rhs rhs_;

 public:
  constexpr Divide(Constant lhs, Rhs rhs) : lhs_(lhs), rhs_(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_ / rhs_(context)) {
    return lhs_ / rhs_(context);
  }

  template<typename T, const char *str>
  constexpr auto gradient() const
      -> decltype(-lhs_ * rhs_.template gradient<T, str>() / (rhs_ * rhs_)) {
    return -lhs_ * rhs_.template gradient<T, str>() / (rhs_ * rhs_);
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator/(Lhs lhs, Rhs rhs)
    -> enable_if_t<
      (IsExpression<Lhs>::value && IsExpression<Rhs>::value)
      || (IsExpression<Lhs>::value && !IsExpression<Rhs>::value &&
          !IsOne<Rhs>::value && !IsZero<Rhs>::value)
      || (!IsExpression<Lhs>::value && IsExpression<Rhs>::value &&
          !IsZero<Lhs>::value),
    Divide<Lhs, Rhs, void>> {
  return {lhs, rhs};
}

template<typename Lhs, typename T>
constexpr auto operator/(Lhs lhs, PlusOne<T> rhs)
    -> enable_if_t<!IsZero<Lhs>::value, Lhs> {
  return lhs;
}

template<typename Lhs, typename T>
constexpr auto operator/(Lhs lhs, MinusOne<T> rhs)
    -> enable_if_t<!IsZero<Lhs>::value, decltype(-lhs)> {
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

#endif
