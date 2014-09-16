// Copyright (c) 2014, Tamas Csala

#ifndef ADD_H_
#define ADD_H_

#include <type_traits>
#include "../variable.hpp"

using real = double;

template<typename Lhs, typename Rhs>
class Add : public Expression<Add<Lhs, Rhs>> {
  const Lhs& lhs;
  const Rhs& rhs;

 public:
  Add(const Lhs& lhs, const Rhs& rhs) : lhs(lhs), rhs(rhs) {}

  auto operator()() const -> decltype(lhs() + rhs()) { return lhs() + rhs(); }

  // if both args have constant gradient, then the result is constant too
  template<typename T, const char *str>
  auto gradient(const Variable<T, str>& v) const
      -> typename std::enable_if<
      std::is_same<T, decltype(lhs.gradient(v))>::value
      && std::is_same<T, decltype(rhs.gradient(v))>::value, T>::type {
    return lhs.gradient(v) + rhs.gradient(v);
  }

  template<typename T, const char *str>
  auto gradient(const Variable<T, str>& v) const
      -> typename std::enable_if<
      !(std::is_same<T, decltype(lhs.gradient(v))>::value
      && std::is_same<T, decltype(rhs.gradient(v))>::value),
      typename Add<decltype(lhs.gradient(v)), decltype(rhs.gradient(v))>::type> {
    return {lhs.gradient(v), rhs.gradient(v)};
  }
};

template<typename Lhs>
class Add<Lhs, real> : public Expression<Add<Lhs, real>> {
  const Lhs& lhs;
  real rhs;

 public:
  Add(const Lhs& lhs, const real& rhs) : lhs(lhs), rhs(rhs) {}

  auto operator()() const -> decltype(lhs() + rhs) { return lhs() + rhs; }

  template<typename T, const char *str>
  auto gradient(const Variable<T, str>& v) const -> decltype(lhs.gradient(v)) {
    return lhs.gradient(v);
  }
};

template<typename Rhs>
class Add<real, Rhs> : public Expression<Add<real, Rhs>> {
  real lhs;
  const Rhs& rhs;

 public:
  Add(const real& lhs, const Rhs& rhs) : lhs(lhs), rhs(rhs) {}
  auto operator()() const -> decltype(lhs + rhs()) { return lhs + rhs(); }

  template<typename T, const char *str>
  auto gradient(const Variable<T, str>& v) const -> decltype(rhs.gradient(v)) {
    return rhs.gradient(v);
  }
};

template<typename Lhs, typename Rhs>
inline Add<Lhs, Rhs> operator+(const Expression<Lhs>& lhs,
                               const Expression<Rhs>& rhs) {
  return {lhs.self(), rhs.self()};
}

template<typename Lhs>
inline Add<Lhs, real> operator+(const Expression<Lhs>& lhs, real rhs) {
  return {lhs.self(), rhs};
}

template<typename Rhs>
inline Add<real, Rhs> operator+(real lhs, const Expression<Rhs>& rhs) {
  return {lhs, rhs.self()};
}

#endif
