#ifndef OPERATORS_BASIC_SQUARE_HPP_
#define OPERATORS_BASIC_SQUARE_HPP_

#include "./multiply.hpp"

template<typename T>
constexpr auto square(T const& t) { return t*t; }

#endif
