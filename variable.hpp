#include "./types.hpp"

#ifndef VARIABLE_HPP_
#define VARIABLE_HPP_

#include <map>
#include <string>
#include <type_traits>

namespace auto_derive {

template<typename T, const char *name_>
struct VariableValue {
  T value;
  constexpr VariableValue(T value) : value(value) {}
};

template<typename T, const char *name_>
class Variable : public Function {
 public:
  constexpr Variable() {}

  constexpr const char * name() const { return name_; }

  template<typename U, typename... Args>
  constexpr T operator()(U val, Args&&... args) const {
    return operator()(args...);
  }

  template<typename... Args>
  constexpr T operator()(VariableValue<T, name_> val, Args&&... args) const {
    return val.value;
  }

  constexpr auto operator=(T value) const {
    return VariableValue<T, name_>(value);
  }

  template<typename U, const char *str>
  friend constexpr auto gradient(Variable self, Variable<U, str> v)
      -> std::enable_if_t<str==name_, PlusOne<U>> {
    return PlusOne<U>{};
  }

  template<typename U, const char *str>
  friend constexpr auto gradient(Variable self, Variable<U, str> v)
      -> std::enable_if_t<str!=name_, Zero<U>> {
    return Zero<U>{};
  }

  friend std::ostream& operator<<(std::ostream& os, Variable const& self) {
    return os << self.name();
  }
};

} // namespace auto_derive

// Variables must be defined outside of functions...
#define DECLARE_VARIABLE(T, X) \
  constexpr char _STRING_OF_VARIABLE_##X[] = #X; \
  constexpr auto_derive::Variable<T, _STRING_OF_VARIABLE_##X> X;

#endif
