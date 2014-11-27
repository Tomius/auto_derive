#ifndef OPERATORS_UNARY_OPERATOR_HPP_
#define OPERATORS_UNARY_OPERATOR_HPP_

#include "../core/variable.hpp"

namespace auto_derive {

template<typename Expr, typename Enable = void>
class UnaryOperator;

template<typename Expr>
class UnaryOperator<Expr, std::enable_if_t<!IsExpression<Expr>()>>
    : public Function  {
 public:
  constexpr UnaryOperator(const Expr expr) : expr_(expr) {}

  constexpr const Constant<Expr>& expr() const { return expr_; }

protected:
  const Constant<Expr> expr_;
};

template<typename Expr>
class UnaryOperator<Expr, std::enable_if_t<IsExpression<Expr>()>>
    : public Function  {
 public:
  constexpr UnaryOperator(Expr expr) : expr_(expr) {}

  constexpr const Expr& expr() const { return expr_; }

protected:
  const Expr expr_;
};


#define USING_UNARY_OPERATOR(Expr)                   \
private:                                             \
  using UnaryOperator<Expr>::expr_;                  \
public:                                              \
  using UnaryOperator<Expr>::UnaryOperator;

}

#endif
