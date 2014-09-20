#ifndef CONSTANT_H_
#define CONSTANT_H_

#include <cmath>
#include <type_traits>

// Ones and zeros are very special in terms of functions, for example
// x + 0 = x. Considering this, if we can recognize that one operand of
// the addition is a zero, then we can optimize an Add node out.
// Same goes for +-1 and Mult/Div. But of course, the real power comes from
// 0 * x = 0. To recognize these, ones zeros must have their own types.

template<typename T>
struct Constant {
  const T value;
  constexpr Constant(T value) : value(value) {}
  constexpr operator T() const { return value; }
};

struct OneType { constexpr OneType() {} };

template <typename T>
constexpr bool IsOne() {
  return std::is_base_of<OneType, T>::value;
}

template<typename T>
struct PlusOne : Constant<T>, OneType {
  constexpr PlusOne() : Constant<T>{1} {}
};

template<typename T>
struct MinusOne : Constant<T>, OneType {
  constexpr MinusOne() : Constant<T>{-1} {}
};

struct ZeroType { constexpr ZeroType() {} };

template <typename T>
constexpr bool IsZero() {
  return std::is_base_of<ZeroType, T>::value;
}

template<typename T>
struct Zero : Constant<T>, ZeroType {
  constexpr Zero() : Constant<T>{0} {}
};

struct NaNType { constexpr NaNType() {} };

template <typename T>
constexpr bool IsNaN() {
  return std::is_base_of<NaNType, T>::value;
}

template<typename T>
struct NaN : Constant<T>, NaNType {
  constexpr NaN() : Constant<T>{NAN} {}
};


#endif
