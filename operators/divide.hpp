#ifndef OPERATORS_DIVIDE_HPP_
#define OPERATORS_DIVIDE_HPP_

#include <map>
#include <string>
#include <type_traits>
#include "../expression.hpp"
#include "./subtract.hpp"
#include "./multiply.hpp"
#include "./unary_minus.hpp"

template<typename Lhs, typename Rhs, typename Enable = void>
class Divide;

template<typename Lhs, typename Rhs>
class Divide<Lhs, Rhs,
    typename std::enable_if<std::is_base_of<Expression, Lhs>::value &&
                            std::is_base_of<Expression, Rhs>::value>::type>
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
    typename std::enable_if<
      std::is_base_of<Expression, Lhs>::value &&
      !std::is_base_of<Expression, Constant>::value>::type>
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
    typename std::enable_if<
      !std::is_base_of<Expression, Constant>::value &&
      std::is_base_of<Expression, Rhs>::value>::type>
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
  constexpr auto gradient() const ->
      decltype(-lhs_ * rhs_.template gradient<T, str>() / (rhs_ * rhs_)) {
    return -lhs_ * rhs_.template gradient<T, str>() / (rhs_ * rhs_);
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator/(Lhs lhs, Rhs rhs) ->
    typename std::enable_if<
        (std::is_base_of<Expression, Lhs>::value &&
      std::is_base_of<Expression, Rhs>::value)
        || (std::is_base_of<Expression, Lhs>::value &&
      !std::is_base_of<Expression, Rhs>::value &&
      !std::is_base_of<OneType, Rhs>::value)
        || (!std::is_base_of<Expression, Lhs>::value &&
      !std::is_base_of<OneType, Lhs>::value &&
      std::is_base_of<Expression, Rhs>::value),
    Divide<Lhs, Rhs, void>>::type {
  return {lhs, rhs};
}

template<typename Lhs, typename T>
constexpr Lhs operator/(Lhs lhs, PlusOne<T> rhs) {
  return lhs;
}

template<typename T, typename Rhs>
constexpr Rhs operator/(PlusOne<T> lhs, Rhs rhs) {
  return rhs;
}

template<typename T, typename U>
constexpr PlusOne<decltype(T{1}/U{1})> operator/(PlusOne<T> lhs, PlusOne<U> rhs) {
  return PlusOne<decltype(T{1}/U{1})>{};
}

template<typename Lhs, typename T>
constexpr Lhs operator/(Lhs lhs, MinusOne<T> rhs) {
  return -lhs;
}

template<typename T, typename Rhs>
constexpr Rhs operator/(MinusOne<T> lhs, Rhs rhs) {
  return -rhs;
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

#endif
