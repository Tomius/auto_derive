#ifndef OPERATORS_ADD_HPP_
#define OPERATORS_ADD_HPP_

#include <type_traits>
#include "../variable.hpp"

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

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const
      -> typename std::enable_if<
        !std::is_same<Zero<T>, decltype(lhs_.template gradient<T, str, v>())>::value &&
        !std::is_same<Zero<T>, decltype(rhs_.template gradient<T, str, v>())>::value,
      decltype(lhs_.template gradient<T, str, v>() +
      rhs_.template gradient<T, str, v>())>::type {
    return lhs_.template gradient<T, str, v>() + rhs_.template gradient<T, str, v>();
  }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const
      -> typename std::enable_if<
        std::is_same<Zero<T>, decltype(lhs_.template gradient<T, str, v>())>::value &&
        !std::is_same<Zero<T>, decltype(rhs_.template gradient<T, str, v>())>::value,
      decltype(rhs_.template gradient<T, str, v>())>::type {
    return rhs_.template gradient<T, str, v>();
  }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const
      -> typename std::enable_if<
        !std::is_same<Zero<T>, decltype(lhs_.template gradient<T, str, v>())>::value &&
        std::is_same<Zero<T>, decltype(rhs_.template gradient<T, str, v>())>::value,
      decltype(lhs_.template gradient<T, str, v>())>::type {
    return lhs_.template gradient<T, str, v>();
  }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const
      -> typename std::enable_if<
        std::is_same<Zero<T>, decltype(lhs_.template gradient<T, str, v>())>::value &&
        std::is_same<Zero<T>, decltype(rhs_.template gradient<T, str, v>())>::value,
      Zero<T>>::type {
    return Zero<T>{};
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

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const -> decltype(lhs_.template gradient<T, str, v>()) {
    return lhs_.template gradient<T, str, v>();
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

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const -> decltype(rhs_.template gradient<T, str, v>()) {
    return rhs_.template gradient<T, str, v>();
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator+(Lhs lhs, Rhs rhs) ->
    typename std::enable_if<
        (std::is_base_of<Expression, Lhs>::value &&
      std::is_base_of<Expression, Rhs>::value)
        || (std::is_base_of<Expression, Lhs>::value &&
      !std::is_base_of<Expression, Rhs>::value)
        || (!std::is_base_of<Expression, Lhs>::value &&
      std::is_base_of<Expression, Rhs>::value),
    Add<Lhs, Rhs, void>>::type {
  return {lhs, rhs};
}

#endif
