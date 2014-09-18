// Copyright (c) 2014, Tamas Csala

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <map>
#include <string>
#include <iostream>
#include <type_traits>
#include "./expression.hpp"

using real = double;

template<typename T, const char *name_>
class Variable : public Expression<Variable<T, name_>> {
 public:
  constexpr Variable() {}

  using value_type = T;

  constexpr const char * name() const { return name_; }

  T operator()(const std::map<std::string, T>& context) const {
    return context.at(name_);
  }

  template<typename U, const char *str, const Variable<T, str>& v>
  constexpr typename std::enable_if<str==name_, U>::type gradient() const { return U(1); }

  template<typename U, const char *str, const Variable<T, str>& v>
  constexpr typename std::enable_if<str!=name_, U>::type gradient() const { return U(0); }
};

// Variables must be defined outside of functions!!
#define DECLARE_VARIABLE(T, X) \
  constexpr char _STRING_OF_VARIABLE_##X[] = #X; \
  constexpr Variable<T, _STRING_OF_VARIABLE_##X> X;

#define VARIABLE(X) decltype(X)::value_type, _STRING_OF_VARIABLE_##X, X

#endif
