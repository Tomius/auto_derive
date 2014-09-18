// Copyright (c) 2014, Tamas Csala

#ifndef EXPRESSION_HPP_
#define EXPRESSION_HPP_

template<typename T>
struct Expression {
  constexpr Expression() {}
  constexpr bool operator==(int) const { return false; }
  constexpr bool operator!=(int) const { return true; }
};

#endif
