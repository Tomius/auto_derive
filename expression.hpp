#ifndef EXPRESSION_HPP_
#define EXPRESSION_HPP_

struct Expression {
  constexpr Expression() {}
};

template<typename T>
struct Constant {
  const T t;
  constexpr Constant(T t) : t(t) {}
  constexpr operator T() const { return t; }
};

template<typename T>
struct One : Constant<T> {
  constexpr One() : Constant<T>{1} {}
};

template<typename T>
struct Zero : Constant<T> {
  constexpr Zero() : Constant<T>{0} {}
};

#endif
