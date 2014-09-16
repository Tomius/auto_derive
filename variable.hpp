// Copyright (c) 2014, Tamas Csala

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include "./expression.hpp"

template<typename T, const char *str>
class Variable : public Expression<Variable<T, str>> {
 public:
  constexpr Variable() {}

  constexpr T operator()() const { return 0; }
  constexpr T gradient(const Variable& v) const { return T(1); }

  template<typename T2, const char *str2>
  constexpr T gradient(const Variable<T2, str2>& v) const { return T(0); }
};

// Variables must be defined outside of functions!!
#define VARIABLE(T, X) \
  char _STRING_OF_##X[] = "#X"; \
  Variable<T, _STRING_OF_##X> X;

#endif
