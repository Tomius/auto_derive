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
  const Lhs lhs_;
  const Rhs rhs_;

 public:
  constexpr Multiply(Lhs lhs, Rhs rhs) : lhs_(lhs), rhs_(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_(context) * rhs_(context)) {
    return lhs_(context) * rhs_(context);
  }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const
      -> decltype(lhs_ * rhs_.template gradient<T, str, v>() +
                 rhs_ * lhs_.template gradient<T, str, v>()) {
    return lhs_ * rhs_.template gradient<T, str, v>() +
    	     rhs_ * lhs_.template gradient<T, str, v>();
  }
};

template<typename Lhs, typename Constant>
class Multiply<Lhs, Constant,
    typename std::enable_if<
      std::is_base_of<Expression<Lhs>, Lhs>::value &&
      !std::is_base_of<Expression<Constant>, Constant>::value>::type>
    : public Expression<Multiply<Lhs, Constant, void>> {

  const Lhs lhs_;
  const Constant rhs_;

 public:
  constexpr Multiply(Lhs lhs, Constant rhs) : lhs_(lhs), rhs_(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_(context) * rhs_) {
    return lhs_(context) * rhs_;
  }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const
      -> decltype(lhs_.template gradient<T, str, v>() * rhs_) {
    return lhs_.template gradient<T, str, v>() * rhs_;
  }
};

template<typename Constant, typename Rhs>
class Multiply<Constant, Rhs,
    typename std::enable_if<
      !std::is_base_of<Expression<Constant>, Constant>::value &&
      std::is_base_of<Expression<Rhs>, Rhs>::value>::type>
    : public Expression<Multiply<Constant, Rhs, void>> {

  const Constant lhs_;
  const Rhs rhs_;

 public:
  constexpr Multiply(Constant lhs, Rhs rhs) : lhs_(lhs), rhs_(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_ * rhs_(context)) {
    return lhs_ * rhs_(context);
  }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const -> decltype(lhs_ * rhs_.template gradient<T, str, v>()) {
    return lhs_ * rhs_.template gradient<T, str, v>();
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator*(Lhs lhs, Rhs rhs_) ->
    typename std::enable_if<
        (std::is_base_of<Expression<Lhs>, Lhs>::value &&
      std::is_base_of<Expression<Rhs>, Rhs>::value)
        || (std::is_base_of<Expression<Lhs>, Lhs>::value &&
      !std::is_base_of<Expression<Rhs>, Rhs>::value)
        || (!std::is_base_of<Expression<Lhs>, Lhs>::value &&
      std::is_base_of<Expression<Rhs>, Rhs>::value),
    Multiply<Lhs, Rhs, void>>::type {
  return {lhs, rhs_};
}

#endif
