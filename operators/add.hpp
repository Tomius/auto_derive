// Copyright (c) 2014, Tamas Csala

#ifndef ADD_H_
#define ADD_H_

#include <type_traits>
#include "../variable.hpp"

template<typename Lhs, typename Rhs>
class Add : public Expression<Add<Lhs, Rhs>> {
  const Lhs lhs;
  const Rhs rhs;

 public:
  constexpr Add(Lhs lhs, Rhs rhs) : lhs(lhs), rhs(rhs) {}

  //constexpr auto operator()() const -> decltype(lhs() + rhs()) { return lhs() + rhs(); }

  // if both args have constant gradient, then the result is the sum of the constants
  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const
      -> typename std::enable_if<
      std::is_same<T, decltype(lhs.template gradient<T, str, v>())>::value
      && std::is_same<T, decltype(rhs.template gradient<T, str, v>())>::value,
      decltype(lhs.template gradient<T, str, v>() + rhs.template gradient<T, str, v>())>::type {
    return lhs.template gradient<T, str, v>() + rhs.template gradient<T, str, v>();
  }

  // if lhs's gradient is zero, and rhs's gradient is not a constant, then the
  // total gradient is rhs's gradient
  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const
      -> typename std::enable_if<lhs.template gradient<T, str, v>() == 0
      && !std::is_same<T, decltype(rhs.template gradient<T, str, v>())>::value,
      decltype(rhs.template gradient<T, str, v>())>::type {
    return rhs.template gradient<T, str, v>();
  }

  // same with rhs zero, lhs not constant
  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const
      -> typename std::enable_if<rhs.template gradient<T, str, v>() == 0
      && !std::is_same<T, decltype(lhs.template gradient<T, str, v>())>::value,
      decltype(lhs.template gradient<T, str, v>())>::type {
    return lhs.template gradient<T, str, v>();
  }

  // if at least one gradient is an expression, and the other one is not zero,
  // then the overall gradient is Add{lhs.gradient(), rhs.gradient()}
  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const
      -> typename std::enable_if<
      !(std::is_same<T, decltype(lhs.template gradient<T, str, v>())>::value
      && std::is_same<T, decltype(rhs.template gradient<T, str, v>())>::value) &&
      lhs.template gradient<T, str, v>() == 0 &&
      rhs.template gradient<T, str, v>() != 0,
      typename Add<decltype(lhs.template gradient<T, str, v>()),
      decltype(rhs.template gradient<T, str, v>())>::type> {
    return {lhs.template gradient<T, str, v>(v), rhs.template gradient<T, str, v>()};
  }
};

template<typename Lhs>
class Add<Lhs, real> : public Expression<Add<Lhs, real>> {
  const Lhs lhs;
  const real rhs;

 public:
  constexpr Add(Lhs lhs, real rhs) : lhs(lhs), rhs(rhs) {}

  //auto operator()() const -> decltype(lhs() + rhs) { return lhs() + rhs; }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const -> decltype(lhs.template gradient<T, str, v>()) {
    return lhs.template gradient<T, str, v>();
  }
};

template<typename Rhs>
class Add<real, Rhs> : public Expression<Add<real, Rhs>> {
  const real lhs;
  const Rhs rhs;

 public:
  constexpr Add(real lhs, Rhs rhs) : lhs(lhs), rhs(rhs) {}
  //auto operator()() const -> decltype(lhs + rhs()) { return lhs + rhs(); }

  template<typename T, const char *str, const Variable<T, str>& v>
  constexpr auto gradient() const -> decltype(rhs.template gradient<T, str, v>()) {
    return rhs.template gradient<T, str, v>();
  }
};

template<typename Lhs, typename Rhs>
constexpr Add<Lhs, Rhs> operator+(const Expression<Lhs>& lhs,
                                  const Expression<Rhs>& rhs) {
  return {lhs.self(), rhs.self()};
}

template<typename Lhs>
constexpr Add<Lhs, real> operator+(const Expression<Lhs>& lhs, real rhs) {
  return {lhs.self(), rhs};
}

template<typename Rhs>
constexpr Add<real, Rhs> operator+(real lhs, const Expression<Rhs>& rhs) {
  return {lhs, rhs.self()};
}

#endif
