#ifndef OPERATORS_SUBTRACT_HPP_
#define OPERATORS_SUBTRACT_HPP_

#include <map>
#include <string>
#include <type_traits>
#include "../expression.hpp"
#include "./unary_minus.hpp"

template<typename Lhs, typename Rhs, typename Enable = void>
class Subtract;

template<typename Lhs, typename Rhs>
class Subtract<Lhs, Rhs,
    typename std::enable_if<std::is_base_of<Expression, Lhs>::value &&
                            std::is_base_of<Expression, Rhs>::value>::type>
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
    typename std::enable_if<
      std::is_base_of<Expression, Lhs>::value &&
      !std::is_base_of<Expression, Constant>::value>::type>
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
  constexpr auto gradient() const -> decltype(lhs_.template gradient<T, str>()) {
    return lhs_.template gradient<T, str>();
  }
};

template<typename Constant, typename Rhs>
class Subtract<Constant, Rhs,
    typename std::enable_if<
      !std::is_base_of<Expression, Constant>::value &&
      std::is_base_of<Expression, Rhs>::value>::type>
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
  constexpr auto gradient() const -> decltype(-rhs_.template gradient<T, str>()) {
    return -rhs_.template gradient<T, str>();
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator-(Lhs lhs, Rhs rhs) ->
    typename std::enable_if<
        (std::is_base_of<Expression, Lhs>::value &&
      std::is_base_of<Expression, Rhs>::value)
        || (std::is_base_of<Expression, Lhs>::value &&
      !std::is_base_of<Expression, Rhs>::value &&
      !std::is_base_of<ZeroType, Rhs>::value)
        || (!std::is_base_of<Expression, Lhs>::value &&
      !std::is_base_of<ZeroType, Lhs>::value &&
      std::is_base_of<Expression, Rhs>::value),
    Subtract<Lhs, Rhs, void>>::type {
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
