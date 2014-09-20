#ifndef EXPRESSION_HPP_
#define EXPRESSION_HPP_

#include <type_traits>

struct Expression {
  constexpr Expression() {}
};

template <typename T>
using IsExpression = std::is_base_of<Expression, T>;

// Maybe I should find a better place for this...
template<bool B, typename T = void>
using enable_if_t = typename std::enable_if<B,T>::type;

#endif
