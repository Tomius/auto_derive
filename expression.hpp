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

// Maybe I should find a better place for this...
template<bool B, typename T = void>
using enable_if_t = typename std::enable_if<B,T>::type;

} // namespace auto_derive

#endif
