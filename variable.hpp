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

  using value_type = T;

  constexpr const char * name() const { return name_; }

  T operator()(const std::map<std::string, T>& context) const {
    return context.at(name_);
  }

  template<typename U, const char *str>
  constexpr typename std::enable_if<str==name_, PlusOne<U>>::type gradient() const {
    return PlusOne<U>{};
  }

  template<typename U, const char *str>
  constexpr typename std::enable_if<str!=name_, Zero<U>>::type gradient() const {
    return Zero<U>{};
  }
};

// Variables must be defined in global namespace...
#define DECLARE_VARIABLE(T, X) \
  constexpr char _STRING_OF_VARIABLE_##X[] = #X; \
  constexpr Variable<T, _STRING_OF_VARIABLE_##X> X;

#define VARIABLE(X) decltype(X)::value_type, _STRING_OF_VARIABLE_##X

#endif
