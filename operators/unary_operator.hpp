#ifndef OPERATORS_UNARY_OPERATOR_HPP_
#define OPERATORS_UNARY_OPERATOR_HPP_

#ifndef AUTO_DERIVE_PROMOTE_INTEGRAL_CONSTANTS
  #define AUTO_DERIVE_PROMOTE_INTEGRAL_CONSTANTS 1
#endif

#include "../core/variable.hpp"

namespace auto_derive {

template<typename Expr, typename Enable = void>
class UnaryOperator;

template<typename Expr>
class UnaryOperator<Expr, std::enable_if_t<IsExpression<Expr>()>>
    : public Function  {
 public:
  constexpr UnaryOperator(Expr const& expr) : expr_(expr) {}

  constexpr Expr const& expr() const { return expr_; }

protected:
  Expr const expr_;
};

template<typename Expr>
class UnaryOperator<Expr, std::enable_if_t<!IsExpression<Expr>()>>
    : public Function  {
 private:
#if AUTO_DERIVE_PROMOTE_INTEGRAL_CONSTANTS
  using ConstantType = Constant<decltype(std::declval<Expr>()*1.0)>;
#else
  using ConstantType = Constant<Expr>;
#endif

 public:
  constexpr UnaryOperator(Expr const& expr) : expr_(expr) {}

  constexpr ConstantType const& expr() const { return expr_; }

 protected:
  ConstantType const expr_;
};

#define __AUTO_DERIVE_USING_UNARY_OPERATOR(Expr)                   \
private:                                             \
  using UnaryOperator<Expr>::expr_;                  \
public:                                              \
  using UnaryOperator<Expr>::UnaryOperator;

}

#endif
