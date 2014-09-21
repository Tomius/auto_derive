#ifndef VARIABLE_HPP_
#define VARIABLE_HPP_

#include <map>
#include <string>
#include <iostream>
#include <type_traits>
#include "./expression.hpp"
#include "./constant.hpp"

namespace auto_derive {

template<typename T, const char *name_>
class Variable : public Expression {
 public:
  constexpr Variable() {}

  constexpr const char * name() const { return name_; }

  T operator()(const std::map<std::string, T>& context) const {
    return context.at(name_);
  }

  template<typename U, const char *str>
  constexpr auto operator%(Variable<U, str> v) const
      -> enable_if_t<str==name_, PlusOne<U>> {
    return PlusOne<U>{};
  }

  template<typename U, const char *str>
  constexpr auto operator%(Variable<U, str> v) const
      -> enable_if_t<str!=name_, Zero<U>> {
    return Zero<U>{};
  }
};

} // namespace auto_derive

// Variables must be defined outside of functions...
#define DECLARE_VARIABLE(T, X) \
  constexpr char _STRING_OF_VARIABLE_##X[] = #X; \
  constexpr auto_derive::Variable<T, _STRING_OF_VARIABLE_##X> X;

#endif
