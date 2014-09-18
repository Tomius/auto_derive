// Copyright (c) 2014, Tamas Csala

#ifndef OPERATORS_SUB_HPP_
#define OPERATORS_SUB_HPP_

#include <type_traits>
#include "../variable.hpp"

template<typename Lhs, typename Rhs, typename Enable = void>
class Sub;

template<typename Lhs, typename Rhs>
class Sub<Lhs, Rhs,
    typename std::enable_if<std::is_base_of<Expression<Lhs>, Lhs>::value &&
                            std::is_base_of<Expression<Rhs>, Rhs>::value>::type>
    : public Expression<Sub<Lhs, Rhs, void>> {
  const Lhs lhs;
  const Rhs rhs;

 public:
  constexpr Sub(Lhs lhs, Rhs rhs) : lhs(lhs), rhs(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs(context) - rhs(context)) {
    return lhs(context) - rhs(context);
  }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const
      -> decltype(lhs.template gradient<T, str, v>() -
        rhs.template gradient<T, str, v>()) {
    return lhs.template gradient<T, str, v>() - rhs.template gradient<T, str, v>();
  }
};

template<typename Lhs, typename Constant>
class Sub<Lhs, Constant,
    typename std::enable_if<
      std::is_base_of<Expression<Lhs>, Lhs>::value &&
      !std::is_base_of<Expression<Constant>, Constant>::value>::type>
    : public Expression<Sub<Lhs, Constant, void>> {

  const Lhs lhs;
  const Constant rhs;

 public:
  constexpr Sub(Lhs lhs, Constant rhs) : lhs(lhs), rhs(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs(context) - rhs) {
    return lhs(context) - rhs;
  }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const -> decltype(lhs.template gradient<T, str, v>()) {
    return lhs.template gradient<T, str, v>();
  }
};

template<typename Constant, typename Rhs>
class Sub<Constant, Rhs,
    typename std::enable_if<
      !std::is_base_of<Expression<Constant>, Constant>::value &&
      std::is_base_of<Expression<Rhs>, Rhs>::value>::type>
    : public Expression<Sub<Constant, Rhs, void>> {

  const Constant lhs;
  const Rhs rhs;

 public:
  constexpr Sub(Constant lhs, Rhs rhs) : lhs(lhs), rhs(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs - rhs(context)) {
    return lhs - rhs(context);
  }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const -> decltype(0 - rhs.template gradient<T, str, v>()) {
    return 0 - rhs.template gradient<T, str, v>();
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator-(Lhs lhs, Rhs rhs) ->
    typename std::enable_if<
        (std::is_base_of<Expression<Lhs>, Lhs>::value &&
      std::is_base_of<Expression<Rhs>, Rhs>::value)
        || (std::is_base_of<Expression<Lhs>, Lhs>::value &&
      !std::is_base_of<Expression<Rhs>, Rhs>::value)
        || (!std::is_base_of<Expression<Lhs>, Lhs>::value &&
      std::is_base_of<Expression<Rhs>, Rhs>::value),
    Sub<Lhs, Rhs, void>>::type {
  return {lhs, rhs};
}

#endif
