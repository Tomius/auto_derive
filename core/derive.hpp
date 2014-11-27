#ifndef DERIVE_HPP_
#define DERIVE_HPP_

namespace auto_derive {

template <int level, typename Expression, typename Variable>
struct DeriveHelper {
  static constexpr auto call(Expression const& expr, Variable const& var);
};

template <typename Expression, typename Variable>
struct DeriveHelper<0, Expression, Variable> {
  static constexpr auto call(Expression const& expr, Variable const& var) {
    return expr;
  }
};

template<int level, typename Expression, typename Variable>
constexpr auto derive(Expression const& expr, Variable const& var) {
  return DeriveHelper<level, Expression, Variable>::call(expr, var);
}

template <int level, typename Expression, typename Variable>
constexpr auto DeriveHelper<level, Expression, Variable>::call(
    Expression const& expr, Variable const& var) {
  return derive<level-1>(derive(expr, var), var);
}

}

#endif
