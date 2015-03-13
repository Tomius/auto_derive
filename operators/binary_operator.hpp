#ifndef OPERATORS_BINARY_OPERATOR_HPP_
#define OPERATORS_BINARY_OPERATOR_HPP_

#ifndef AUTO_DERIVE_PROMOTE_INTEGRAL_CONSTANTS
  #define AUTO_DERIVE_PROMOTE_INTEGRAL_CONSTANTS 1
#endif

#include "../core/variable.hpp"

namespace auto_derive {

template<typename Lhs, typename Rhs, typename Enable = void>
class BinaryOperator;

template<typename Lhs, typename Rhs>
class BinaryOperator<Lhs, Rhs,
    enable_if_t<IsExpression<Lhs>() && IsExpression<Rhs>()>>
    : public Function {
 public:
  constexpr BinaryOperator(Lhs const& lhs, Rhs const& rhs)
      : lhs_(lhs), rhs_(rhs) {}

  constexpr Lhs const& lhs() const { return lhs_; }
  constexpr Rhs const& rhs() const { return rhs_; }

 protected:
  Lhs const lhs_;
  Rhs const rhs_;
};

template<typename Lhs, typename Rhs>
class BinaryOperator<Lhs, Rhs,
    enable_if_t<!IsExpression<Lhs>() && IsExpression<Rhs>()>>
    : public Function {
 private:
#if AUTO_DERIVE_PROMOTE_INTEGRAL_CONSTANTS
  using ConstantType = Constant<decltype(std::declval<Lhs>()*1.0)>;
#else
  using ConstantType = Constant<Lhs>;
#endif

 public:
  constexpr BinaryOperator(Lhs const& lhs, Rhs const& rhs)
      : lhs_(lhs), rhs_(rhs) {}

  constexpr ConstantType const& lhs() const { return lhs_; }
  constexpr Rhs const& rhs() const { return rhs_; }

 protected:
  ConstantType const lhs_;
  Rhs const rhs_;
};

template<typename Lhs, typename Rhs>
class BinaryOperator<Lhs, Rhs,
    enable_if_t<IsExpression<Lhs>() && !IsExpression<Rhs>()>>
    : public Function {
 private:
#if AUTO_DERIVE_PROMOTE_INTEGRAL_CONSTANTS
  using ConstantType = Constant<decltype(std::declval<Rhs>()*1.0)>;
#else
  using ConstantType = Constant<Lhs>;
#endif

 public:
  constexpr BinaryOperator(Lhs const& lhs, Rhs const& rhs)
      : lhs_(lhs), rhs_(rhs) {}

  constexpr Lhs const& lhs() const { return lhs_; }
  constexpr ConstantType const& rhs() const { return rhs_; }

 protected:
  Lhs const lhs_;
  ConstantType const rhs_;
};

#define __AUTO_DERIVE_USING_BINARY_OPERATOR(Lhs, Rhs)                \
private:                                               \
  using BinaryOperator<Lhs, Rhs>::lhs_;                \
  using BinaryOperator<Lhs, Rhs>::rhs_;                \
public:                                                \
 using BinaryOperator<Lhs, Rhs>::BinaryOperator;

}

#endif
