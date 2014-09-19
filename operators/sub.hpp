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
  const Lhs lhs_;
  const Rhs rhs_;

 public:
  constexpr Sub(Lhs lhs_, Rhs rhs_) : lhs_(lhs_), rhs_(rhs_) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_(context) - rhs_(context)) {
    return lhs_(context) - rhs_(context);
  }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const
      -> decltype(lhs_.template gradient<T, str, v>() -
        rhs_.template gradient<T, str, v>()) {
    return lhs_.template gradient<T, str, v>() - rhs_.template gradient<T, str, v>();
  }
};

template<typename Lhs, typename Constant>
class Sub<Lhs, Constant,
    typename std::enable_if<
      std::is_base_of<Expression<Lhs>, Lhs>::value &&
      !std::is_base_of<Expression<Constant>, Constant>::value>::type>
    : public Expression<Sub<Lhs, Constant, void>> {

  const Lhs lhs_;
  const Constant rhs_;

 public:
  constexpr Sub(Lhs lhs_, Constant rhs_) : lhs_(lhs_), rhs_(rhs_) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_(context) - rhs_) {
    return lhs_(context) - rhs_;
  }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const -> decltype(lhs_.template gradient<T, str, v>()) {
    return lhs_.template gradient<T, str, v>();
  }
};

template<typename Constant, typename Rhs>
class Sub<Constant, Rhs,
    typename std::enable_if<
      !std::is_base_of<Expression<Constant>, Constant>::value &&
      std::is_base_of<Expression<Rhs>, Rhs>::value>::type>
    : public Expression<Sub<Constant, Rhs, void>> {

  const Constant lhs_;
  const Rhs rhs_;

 public:
  constexpr Sub(Constant lhs, Rhs rhs) : lhs_(lhs), rhs_(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_ - rhs_(context)) {
    return lhs_ - rhs_(context);
  }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const -> decltype(0 - rhs_.template gradient<T, str, v>()) {
    return 0 - rhs_.template gradient<T, str, v>();
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
