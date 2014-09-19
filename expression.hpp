#ifndef EXPRESSION_HPP_
#define EXPRESSION_HPP_

struct Expression {
  constexpr Expression() {}
};

template<typename T>
struct Constant {
  const T value;
  constexpr Constant(T value) : value(value) {}
  constexpr operator T() const { return value; }
};

struct OneType { constexpr OneType() {} };

template<typename T>
struct One : Constant<T>, OneType {
  constexpr One() : Constant<T>{1} {}
};

struct ZeroType { constexpr ZeroType() {} };

template<typename T>
struct Zero : Constant<T>, ZeroType {
  constexpr Zero() : Constant<T>{0} {}
};

#endif
