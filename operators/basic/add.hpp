#ifndef OPERATORS_ADD_HPP_
#define OPERATORS_ADD_HPP_

#include "../../variable.hpp"

template<typename Lhs, typename Rhs, typename Enable = void>
class Add;

template<typename Lhs, typename Rhs>
class Add<Lhs, Rhs, enable_if_t<IsExpression<Lhs>() && IsExpression<Rhs>()>>
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
  constexpr auto gradient(Variable<T, str> v) const
      -> decltype(lhs_.gradient(v) + rhs_.gradient(v)) {
    return lhs_.gradient(v) + rhs_.gradient(v);
  }
};

template<typename Lhs, typename Constant>
class Add<Lhs, Constant,
    enable_if_t<IsExpression<Lhs>() && !IsExpression<Constant>()>>
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
  constexpr auto gradient(Variable<T, str> v) const
      -> decltype(lhs_.gradient(v)) {
    return lhs_.gradient(v);
  }
};

template<typename Constant, typename Rhs>
class Add<Constant, Rhs,
    enable_if_t<!IsExpression<Constant>() && IsExpression<Rhs>()>>
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
  constexpr auto gradient(Variable<T, str> v) const
      -> decltype(rhs_.gradient(v)) {
    return rhs_.gradient(v);
  }
};

template<typename Lhs, typename Rhs>
constexpr auto operator+(Lhs lhs, Rhs rhs)
    -> enable_if_t<
      (IsExpression<Lhs>() && IsExpression<Rhs>())
      || (IsExpression<Lhs>() && !IsExpression<Rhs>() && !IsZero<Rhs>())
      || (!IsExpression<Lhs>() && IsExpression<Rhs>() && !IsZero<Lhs>()),
    Add<Lhs, Rhs, void>> {
  return {lhs, rhs};
}

template<typename Lhs, typename T>
constexpr Lhs operator+(Lhs lhs, Zero<T> rhs) {
  return lhs;
}

template<typename T, typename Rhs>
constexpr Rhs operator+(Zero<T> lhs, Rhs rhs) {
  return rhs;
}

template<typename T, typename U>
constexpr Zero<decltype(T{0}+U{0})> operator+(Zero<T> lhs, Zero<U> rhs) {
  return Zero<decltype(T{0}+U{0})>{};
}

template<typename T, typename U>
constexpr Zero<decltype(T{1}+U{-1})> operator+(PlusOne<T> lhs, MinusOne<U> rhs) {
  return Zero<decltype(T{1}+U{-1})>{};
}

template<typename T, typename U>
constexpr Zero<decltype(T{-1}+U{1})> operator+(MinusOne<T> lhs, PlusOne<U> rhs) {
  return Zero<decltype(T{-1}+U{1})>{};
}

#endif
