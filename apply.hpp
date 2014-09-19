#ifndef APPLY_H_
#define APPLY_H_

#include "./variable.hpp"

// needed to differentiate f(g(x))
template<typename Expr, const char * variable, typename Value>
class Substitute : public Expression{
  const Expr expr_;
  const Value value_;

 public:
  constexpr Substitute(Expr expr, Value value) : expr_(expr), value_(value) {}

  template<typename T>
  auto operator()(const std::map<std::string, T>& context) const
      -> decltype(expr_(context)) {
    T last_value = context.at(variable);
    const_cast<std::map<std::string, T>&>(context)[variable] = value_(context);
    T ret = expr_(context);
    const_cast<std::map<std::string, T>&>(context)[variable] = last_value;
    return ret;
  }
};

#endif
