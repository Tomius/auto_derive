#ifndef VARIABLE_HPP_
#define VARIABLE_HPP_

#include <map>
#include <string>
#include <iostream>
#include <type_traits>
#include "./expression.hpp"
#include "./constant.hpp"

template<typename T, const char *name_>
class Variable : public Expression {
 public:
  constexpr Variable() {}

  constexpr const char * name() const { return name_; }

  T operator()(const std::map<std::string, T>& context) const {
    return context.at(name_);
  }

  template<typename U, const char *str>
  constexpr enable_if_t<str==name_, PlusOne<U>> gradient(Variable<U, str> v) const {
    return PlusOne<U>{};
  }

  template<typename U, const char *str>
  constexpr enable_if_t<str!=name_, Zero<U>> gradient(Variable<U, str> v) const {
    return Zero<U>{};
  }
};

// Variables must be defined outside of functions...
#define DECLARE_VARIABLE(T, X) \
  constexpr char _STRING_OF_VARIABLE_##X[] = #X; \
  constexpr Variable<T, _STRING_OF_VARIABLE_##X> X;

#endif
