#ifndef OPERATORS_ADD_HPP_
#define OPERATORS_ADD_HPP_

#include <map>
#include <string>
#include <type_traits>
#include "../expression.hpp"

template<typename Lhs, typename Rhs, typename Enable = void>
class Add;

template<typename Lhs, typename Rhs>
class Add<Lhs, Rhs,
    typename std::enable_if<std::is_base_of<Expression, Lhs>::value &&
                            std::is_base_of<Expression, Rhs>::value>::type>
    : public Expression {
  const Lhs lhs_;
  const Rhs rhs_;

 public:
  constexpr Add(Lhs lhs, Rhs rhs) : lhs_(lhs), rhs_(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_(context) + rhs_(context)) {
    return lhs_(context) + rhs_(context);
  }

  template<typename T, const char *str>
  constexpr auto gradient() const
      -> decltype(lhs_.template gradient<T, str>() +
                  rhs_.template gradient<T, str>()) {
    return lhs_.template gradient<T, str>() + rhs_.template gradient<T, str>();
  }
};

template<typename Lhs, typename Constant>
class Add<Lhs, Constant,
    typename std::enable_if<
  	  std::is_base_of<Expression, Lhs>::value &&
      !std::is_base_of<Expression, Constant>::value>::type>
    : public Expression {

  const Lhs lhs_;
  const Constant rhs_;

 public:
  constexpr Add(Lhs lhs, Constant rhs) : lhs_(lhs), rhs_(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_(context) + rhs_) {
    return lhs_(context) + rhs_;
  }

  template<typename T, const char *str>
  constexpr auto gradient() const -> decltype(lhs_.template gradient<T, str>()) {
    return lhs_.template gradient<T, str>();
  }
};

template<typename Constant, typename Rhs>
class Add<Constant, Rhs,
    typename std::enable_if<
      !std::is_base_of<Expression, Constant>::value &&
      std::is_base_of<Expression, Rhs>::value>::type>
    : public Expression {

  const Constant lhs_;
  const Rhs rhs_;

 public:
  constexpr Add(Constant lhs, Rhs rhs) : lhs_(lhs), rhs_(rhs) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(lhs_ + rhs_(context)) {
    return lhs_ + rhs_(context);
  }

  template<typename T, const char *str>
  constexpr auto gradient() const -> decltype(rhs_.template gradient<T, str>()) {
    return rhs_.template gradient<T, str>();
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator+(Lhs lhs, Rhs rhs) ->
    typename std::enable_if<
        (std::is_base_of<Expression, Lhs>::value &&
      std::is_base_of<Expression, Rhs>::value)
        || (std::is_base_of<Expression, Lhs>::value &&
      !std::is_base_of<Expression, Rhs>::value &&
      !std::is_base_of<ZeroType, Rhs>::value)
        || (!std::is_base_of<Expression, Lhs>::value &&
      !std::is_base_of<ZeroType, Lhs>::value &&
      std::is_base_of<Expression, Rhs>::value),
    Add<Lhs, Rhs, void>>::type {
  return {lhs, rhs};
}

template<typename Lhs, typename Rhs>
constexpr auto operator+(Lhs lhs, Rhs rhs) ->
    typename std::enable_if<
      std::is_base_of<Expression, Lhs>::value &&
      !std::is_base_of<Expression, Rhs>::value &&
      std::is_base_of<ZeroType, Rhs>::value,
    Lhs>::type {
  return lhs;
}

template<typename Lhs, typename Rhs>
constexpr auto operator+(Lhs lhs, Rhs rhs) ->
    typename std::enable_if<
      !std::is_base_of<Expression, Lhs>::value &&
      std::is_base_of<ZeroType, Lhs>::value &&
      std::is_base_of<Expression, Rhs>::value,
    Rhs>::type {
  return rhs;
}

#endif
