#ifndef OPERATORS_SUBTRACT_HPP_
#define OPERATORS_SUBTRACT_HPP_

#include "../variable.hpp"
#include "./unary_minus.hpp"

template<typename Lhs, typename Rhs, typename Enable = void>
class Subtract;

template<typename Lhs, typename Rhs>
class Subtract<Lhs, Rhs,
    enable_if_t<IsExpression<Lhs>::value && IsExpression<Rhs>::value>>
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
  constexpr auto gradient() const
      -> decltype(lhs_.template gradient<T, str>() -
                  rhs_.template gradient<T, str>()) {
    return lhs_.template gradient<T, str>() - rhs_.template gradient<T, str>();
  }
};

template<typename Lhs, typename Constant>
class Subtract<Lhs, Constant,
    enable_if_t<IsExpression<Lhs>::value && !IsExpression<Constant>::value>>
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
  constexpr auto gradient() const
      -> decltype(lhs_.template gradient<T, str>()) {
    return lhs_.template gradient<T, str>();
  }
};

template<typename Constant, typename Rhs>
class Subtract<Constant, Rhs,
    enable_if_t<!IsExpression<Constant>::value && IsExpression<Rhs>::value>>
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
  constexpr auto gradient() const
      -> decltype(-rhs_.template gradient<T, str>()) {
    return -rhs_.template gradient<T, str>();
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator-(Lhs lhs, Rhs rhs)
    -> enable_if_t<
        (IsExpression<Lhs>::value && IsExpression<Rhs>::value)
        || (IsExpression<Lhs>::value && !IsExpression<Rhs>::value &&
            !IsZero<Rhs>::value)
        || (!IsExpression<Lhs>::value && IsExpression<Rhs>::value &&
            !IsZero<Lhs>::value),
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

#endif
