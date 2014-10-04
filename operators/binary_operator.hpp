#ifndef OPERATORS_BINARY_OPERATOR_HPP_
#define OPERATORS_BINARY_OPERATOR_HPP_

#include "../types.hpp"

namespace auto_derive {

template<typename Lhs, typename Rhs, typename Enable = void>
class BinaryOperator;

template<typename Lhs, typename Rhs>
class BinaryOperator<Lhs, Rhs,
    std::enable_if_t<IsExpression<Lhs>() && IsExpression<Rhs>()>>
    : public Function {
 public:

  constexpr BinaryOperator(Lhs lhs, Rhs rhs) : lhs_(lhs), rhs_(rhs) {}

  constexpr const Lhs& lhs() const { return lhs_; }
  constexpr const Rhs& rhs() const { return rhs_; }

 protected:
  const Lhs lhs_;
  const Rhs rhs_;
};

template<typename Lhs, typename Rhs>
class BinaryOperator<Lhs, Rhs,
    std::enable_if_t<!IsExpression<Lhs>() && IsExpression<Rhs>()>>
    : public Function {
 public:

  constexpr BinaryOperator(Lhs lhs, Rhs rhs) : lhs_(lhs), rhs_(rhs) {}

  constexpr const Constant<Lhs>& lhs() const { return lhs_; }
  constexpr const Rhs& rhs() const { return rhs_; }

 protected:
  const Constant<Lhs> lhs_;
  const Rhs rhs_;
};

template<typename Lhs, typename Rhs>
class BinaryOperator<Lhs, Rhs,
    std::enable_if_t<IsExpression<Lhs>() && !IsExpression<Rhs>()>>
    : public Function {
 public:

  constexpr BinaryOperator(Lhs lhs, Rhs rhs) : lhs_(lhs), rhs_(rhs) {}

  constexpr const Lhs& lhs() const { return lhs_; }
  constexpr const Constant<Rhs>& rhs() const { return rhs_; }

 protected:
  const Lhs lhs_;
  const Constant<Rhs> rhs_;
};

#define USING_BINARY_OPERATOR(Lhs, Rhs)                \
private:                                               \
  using BinaryOperator<Lhs, Rhs>::lhs_;                \
  using BinaryOperator<Lhs, Rhs>::rhs_;                \
public:                                                \
 using BinaryOperator<Lhs, Rhs>::BinaryOperator;

}

#endif
