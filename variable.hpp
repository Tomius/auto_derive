// Copyright (c) 2014, Tamas Csala

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <type_traits>
#include "./expression.hpp"

template<typename T, const char *name_>
class Variable : public Expression<Variable<T, name_>> {
 public:
  constexpr Variable() {}

  constexpr const char * name() const { return name_; }

  //constexpr Variable operator()() const { return 0; }

  template<typename U, const char *str, const Variable<T, str>& v>
  constexpr typename std::enable_if<str==name_, T>::type gradient() const { return T(1); }

  template<typename U, const char *str, const Variable<T, str>& v>
  constexpr typename std::enable_if<str!=name_, T>::type gradient() const { return T(0); }
};

// Variables must be defined outside of functions!!
#define VARIABLE(T, X) \
  constexpr char _STRING_OF_##X[] = "#X"; \
  constexpr Variable<T, _STRING_OF_##X> X;

#endif
