#ifndef DERIVE_HPP_
#define DERIVE_HPP_

#include "./variable.hpp"

namespace auto_derive {

template <int level, typename Expression, typename Variable>
struct DeriveHelper {
  static constexpr auto call(Expression const& expr, Variable var);
};

template <typename Expression, typename Variable>
struct DeriveHelper<0, Expression, Variable> {
  static constexpr auto call(Expression const& expr, Variable var) {
    return expr;
  }
};

template <int level, typename Expression, typename Variable>
constexpr auto DeriveHelper<level, Expression, Variable>::call(
    Expression const& expr, Variable var) {
  return derive<level-1>(derive(expr, var), var);
}

}

// According to the C++ Standard 14.8.1.6, ADL won't work for
// function templates with explicit template arguments.
// If we defined derive<n> in namespace auto_derive, then
// calling derive<2>(f, x), without a previous using statement
// would be ill-formed, even though derive(f, x) compiles.
//
// As a compromise, derive<n> is defined in the global namespace.
// This shouldn't be a problem though, as its last parameter is
// auto_derive::Variable, so its unlikely to clash with other
// derive functions.
template<int level, typename Expression, typename... Varargs>
constexpr auto derive(Expression const& expr,
                      auto_derive::Variable<Varargs...> var) {
  using auto_derive::DeriveHelper;
  using auto_derive::Variable;
  return DeriveHelper<level, Expression, Variable<Varargs...>>::call(expr, var);
}

#endif
