#ifndef EXPRESSION_HPP_
#define EXPRESSION_HPP_

#include <type_traits>

namespace auto_derive {

struct Expression {
  constexpr Expression() {}
};

template <typename T>
constexpr bool IsExpression() {
  return std::is_base_of<Expression, T>::value;
}

} // namespace auto_derive

#endif
