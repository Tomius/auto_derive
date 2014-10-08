#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <cmath>
#include <iostream>
#include <type_traits>
#include "./parenthesis.hpp"

namespace auto_derive {

// Everything that can be evaluated and derived is an expression
struct Expression {
  constexpr Expression() {}
};

template <typename T>
constexpr bool IsExpression() {
  return std::is_base_of<Expression, T>::value;
}

// Everything is a function, that depends on a variable
struct Function : public Expression {
  constexpr Function() {}
};

template <typename T>
constexpr bool IsFunction() {
  return std::is_base_of<Function, T>::value;
}

// A constant is a wrapper for a scalar.
template<typename T>
struct Constant : public Expression {
  const T value;
  constexpr Constant(T value) : value(value) {}
  constexpr operator T() const { return value; }

  template<typename... Args>
  constexpr auto operator()(Args&&... args) const {
    return value;
  }

  friend std::ostream& operator<<(std::ostream& os, Constant self) {
    return os << self.value;
  }
};

// Ones and zeros are very special in terms of functions, for example
// x + 0 = x. Considering this, if we can recognize that one operand of
// the addition is a zero, then we can optimize an Add node out.
// Same goes for +-1 and Mult/Div. But of course, the real power comes from
// 0 * x = 0. To recognize these, ones and zeros must have their own types.

// +-1

struct OneType { constexpr OneType() {} };

template <typename T>
constexpr bool IsOne() {
  return std::is_base_of<OneType, T>::value;
}

// +1

struct PlusOneType : OneType { constexpr PlusOneType() {} };

template<typename T>
struct PlusOne : Constant<T>, PlusOneType {
  constexpr PlusOne() : Constant<T>{1} {}
};

template <typename T>
constexpr bool IsPlusOne() {
  return std::is_base_of<PlusOneType, T>::value;
}

// -1

struct MinusOneType : OneType { constexpr MinusOneType() {} };

template<typename T>
struct MinusOne : Constant<T>, MinusOneType {
  constexpr MinusOne() : Constant<T>{-1} {}
};

template <typename T>
constexpr bool IsMinusOne() {
  return std::is_base_of<MinusOneType, T>::value;
}

// 0

struct ZeroType { constexpr ZeroType() {} };

template <typename T>
constexpr bool IsZero() {
  return std::is_base_of<ZeroType, T>::value;
}

template<typename T>
struct Zero : Constant<T>, ZeroType {
  constexpr Zero() : Constant<T>{0} {}
};

} // namespace auto_derive

#include "./variable.hpp"

namespace auto_derive {

// The derivation of a Constant have to defined out-of-line,
// since this depends on the variable class, and the variable class depends
// on the constant type.
template<typename T, typename Variable>
constexpr auto gradient(Constant<T> self, Variable v) {
  return Zero<T>{};
}

}


#endif
