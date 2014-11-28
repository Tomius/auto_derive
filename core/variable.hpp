#include "./types.hpp"

#ifndef VARIABLE_HPP_
#define VARIABLE_HPP_

#include <map>
#include <string>
#include <type_traits>

namespace auto_derive {

template<typename T, typename Name>
struct VariableValue { T value; };

template<typename T, typename Name>
class Variable : public Function {
 public:
  constexpr Variable(const char* name) : name_(name) {}

  // f(5) style eval
  constexpr auto operator()(T value) const {
    return value;
  }

  // f(y=2, x=...) style eval where y is not the current variable
  template<typename U, typename Name2, typename... Args>
  constexpr auto operator()(VariableValue<U, Name2> val, Args... args) const {
    return operator()(args...);
  }

  // f(y=2, x=...) style eval on y
  template<typename... Args>
  constexpr T operator()(VariableValue<T, Name> val, Args... args) const {
    return val.value;
  }

  // f() eval (the terminating point for partial evaluation)
  template<typename... Args>
  constexpr Variable operator()() const {
    return *this;
  }

  // x=2 returns a VariableValue, so it can used for f(x=2) like eval
  constexpr auto operator=(T value) const {
    return VariableValue<T, Name>{value};
  }

  // dx/dx
  friend constexpr auto derive(Variable self, Variable v) {
    return PlusOne<T>{};
  }

  // dx/dy
  template<typename U, typename Name2>
  friend constexpr auto derive(Variable self, Variable<U, Name2> v) {
    return Zero<U>{};
  }

  // cout << x
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
