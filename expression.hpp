// Copyright (c) 2014, Tamas Csala

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

template<typename T>
struct Expression {
  constexpr const T& self() const {
    return static_cast<const T&>(*this);
  }
};

#endif
