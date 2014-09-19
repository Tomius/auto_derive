#ifndef OPERATORS_MULTIPLY_HPP_
#define OPERATORS_MULTIPLY_HPP_

#include <map>
#include <string>
#include <type_traits>
#include "../expression.hpp"
#include "./add.hpp"

template<typename Lhs, typename Rhs, typename Enable = void>
class Multiply;

template<typename Lhs, typename Rhs>
class Multiply<Lhs, Rhs,
    typename std::enable_if<std::is_base_of<Expression, Lhs>::value &&
                            std::is_base_of<Expression, Rhs>::value>::type>
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
  constexpr auto gradient() const
      -> decltype(lhs_ * rhs_.template gradient<T, str>() +
                 rhs_ * lhs_.template gradient<T, str>()) {
    return lhs_ * rhs_.template gradient<T, str>() +
    	     rhs_ * lhs_.template gradient<T, str>();
  }
};

template<typename Lhs, typename Constant>
class Multiply<Lhs, Constant,
    typename std::enable_if<
      std::is_base_of<Expression, Lhs>::value &&
      !std::is_base_of<Expression, Constant>::value>::type>
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
  constexpr auto gradient() const
      -> decltype(lhs_.template gradient<T, str>() * rhs_) {
    return lhs_.template gradient<T, str>() * rhs_;
  }
};

template<typename Constant, typename Rhs>
class Multiply<Constant, Rhs,
    typename std::enable_if<
      !std::is_base_of<Expression, Constant>::value &&
      std::is_base_of<Expression, Rhs>::value>::type>
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
  constexpr auto gradient() const -> decltype(lhs_ * rhs_.template gradient<T, str>()) {
    return lhs_ * rhs_.template gradient<T, str>();
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator*(Lhs lhs, Rhs rhs) ->
    typename std::enable_if<
        (std::is_base_of<Expression, Lhs>::value &&
      std::is_base_of<Expression, Rhs>::value)
        || (std::is_base_of<Expression, Lhs>::value &&
      !std::is_base_of<Expression, Rhs>::value &&
      !std::is_base_of<OneType, Rhs>::value &&
      !std::is_base_of<ZeroType, Rhs>::value)
        || (!std::is_base_of<Expression, Lhs>::value &&
      !std::is_base_of<OneType, Lhs>::value &&
      !std::is_base_of<ZeroType, Lhs>::value &&
      std::is_base_of<Expression, Rhs>::value),
    Multiply<Lhs, Rhs, void>>::type {
  return {lhs, rhs};
}

template<typename Lhs, typename T>
constexpr auto operator*(Lhs lhs, PlusOne<T> rhs) ->
    typename std::enable_if<!std::is_base_of<ZeroType, Lhs>::value, Lhs>::type {
  return lhs;
}

template<typename T, typename Rhs>
constexpr auto operator*(PlusOne<T> lhs, Rhs rhs) ->
    typename std::enable_if<!std::is_base_of<ZeroType, Rhs>::value, Rhs>::type {
  return rhs;
}

template<typename T, typename U>
constexpr PlusOne<decltype(T{1}*U{1})> operator*(PlusOne<T> lhs, PlusOne<U> rhs) {
  return PlusOne<decltype(T{1}*U{1})>{};
}

template<typename Lhs, typename T>
constexpr auto operator*(Lhs lhs, MinusOne<T> rhs) ->
    typename std::enable_if<!std::is_base_of<ZeroType, Lhs>::value,
    decltype(-lhs)>::type {
  return -lhs;
}

template<typename T, typename Rhs>
constexpr auto operator*(MinusOne<T> lhs, Rhs rhs) ->
    typename std::enable_if<!std::is_base_of<ZeroType, Rhs>::value,
    decltype(-rhs)>::type {
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
