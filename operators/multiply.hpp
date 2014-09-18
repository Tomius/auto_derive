// Copyright (c) 2014, Tamas Csala

#ifndef OPERATORS_MULTIPLY_HPP_
#define OPERATORS_MULTIPLY_HPP_

#include <type_traits>
#include "../variable.hpp"

template<typename Lhs, typename Rhs, typename Enable = void>
class Multiply;

template<typename Lhs, typename Rhs>
class Multiply<Lhs, Rhs,
    typename std::enable_if<std::is_base_of<Expression<Lhs>, Lhs>::value &&
                            std::is_base_of<Expression<Rhs>, Rhs>::value>::type>
    : public Expression<Multiply<Lhs, Rhs, void>> {
  const Lhs lhs;
  const Rhs rhs;

 public:
  constexpr Multiply(Lhs lhs, Rhs rhs) : lhs(lhs), rhs(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs(context) * rhs(context)) {
    return lhs(context) * rhs(context);
  }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const
      -> decltype(lhs * rhs.template gradient<T, str, v>() +
                 rhs * lhs.template gradient<T, str, v>()) {
    return lhs * rhs.template gradient<T, str, v>() +
    	     rhs * lhs.template gradient<T, str, v>();
  }
};

template<typename Lhs, typename Constant>
class Multiply<Lhs, Constant,
    typename std::enable_if<
      std::is_base_of<Expression<Lhs>, Lhs>::value &&
      !std::is_base_of<Expression<Constant>, Constant>::value>::type>
    : public Expression<Multiply<Lhs, Constant, void>> {

  const Lhs lhs;
  const Constant rhs;

 public:
  constexpr Multiply(Lhs lhs, Constant rhs) : lhs(lhs), rhs(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs(context) * rhs) {
    return lhs(context) * rhs;
  }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const
      -> decltype(lhs.template gradient<T, str, v>() * rhs) {
    return lhs.template gradient<T, str, v>() * rhs;
  }
};

template<typename Constant, typename Rhs>
class Multiply<Constant, Rhs,
    typename std::enable_if<
      !std::is_base_of<Expression<Constant>, Constant>::value &&
      std::is_base_of<Expression<Rhs>, Rhs>::value>::type>
    : public Expression<Multiply<Constant, Rhs, void>> {

  const Constant lhs;
  const Rhs rhs;

 public:
  constexpr Multiply(Constant lhs, Rhs rhs) : lhs(lhs), rhs(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs * rhs(context)) {
    return lhs * rhs(context);
  }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const -> decltype(lhs * rhs.template gradient<T, str, v>()) {
    return lhs * rhs.template gradient<T, str, v>();
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator*(Lhs lhs, Rhs rhs) ->
    typename std::enable_if<
        (std::is_base_of<Expression<Lhs>, Lhs>::value &&
      std::is_base_of<Expression<Rhs>, Rhs>::value)
        || (std::is_base_of<Expression<Lhs>, Lhs>::value &&
      !std::is_base_of<Expression<Rhs>, Rhs>::value)
        || (!std::is_base_of<Expression<Lhs>, Lhs>::value &&
      std::is_base_of<Expression<Rhs>, Rhs>::value),
    Multiply<Lhs, Rhs, void>>::type {
  return {lhs, rhs};
}

#endif
