#include "./types.hpp"

#ifndef VARIABLE_HPP_
#define VARIABLE_HPP_

#include <map>
#include <string>
#include <type_traits>

namespace auto_derive {

template<typename T, typename Name>
struct VariableValue {
  T value;
  constexpr VariableValue(T value) : value(value) {}
};

template<typename T, typename Name>
class Variable : public Function {
 public:
  constexpr Variable(const char* name) : name_(name) {}

  constexpr auto operator()(T value) const {
    return value;
  }

  template<typename U, typename Name2, typename... Args>
  constexpr auto operator()(VariableValue<U, Name2> val, Args&&... args) const {
    return operator()(std::forward<Args>(args)...);
  }

  template<typename... Args>
  constexpr T operator()(VariableValue<T, Name> val, Args&&... args) const {
    return val.value;
  }

  template<typename... Args>
  constexpr Variable operator()() const {
    return *this;
  }

  constexpr auto operator=(T value) const {
    return VariableValue<T, Name>(value);
  }

  friend constexpr auto derive(Variable self, Variable v) {
    return PlusOne<T>{};
  }

  template<typename U, typename Name2>
  friend constexpr auto derive(Variable self, Variable<U, Name2> v) {
    return Zero<U>{};
  }

  friend std::ostream& operator<<(std::ostream& os, Variable self) {
    return os << self.name_;
  }

 private:
  const char* name_;
};

} // namespace auto_derive

#define DECLARE_VARIABLE(T, X) \
  constexpr auto_derive::Variable<T, struct X> X{#X};

#endif
