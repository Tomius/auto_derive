#ifndef EXPRESSION_HPP_
#define EXPRESSION_HPP_

struct Expression {
  constexpr Expression() {}
};

// Ones and zeros are very special in terms of functions, for example
// x + 0 = x. Considering this, if we can recognize that one operand of
// the addition is a zero, then we can optimize an Add node out.
// Same goes for +-1 and Mult/Div. But this can only be done if ones zeros
// have their own types.

template<typename T>
struct Constant {
  const T value;
  constexpr Constant(T value) : value(value) {}
  constexpr operator T() const { return value; }
};

struct OneType { constexpr OneType() {} };

template<typename T>
struct PlusOne : Constant<T>, OneType {
  constexpr PlusOne() : Constant<T>{1} {}
};

template<typename T>
struct MinusOne : Constant<T>, OneType {
  constexpr MinusOne() : Constant<T>{-1} {}
};

struct ZeroType { constexpr ZeroType() {} };

template<typename T>
struct Zero : Constant<T>, ZeroType {
  constexpr Zero() : Constant<T>{0} {}
};

#endif
