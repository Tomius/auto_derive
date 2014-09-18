// Copyright (c) 2014, Tamas Csala

#include <iostream>
#include <type_traits>
#include "operators/add.hpp"

using real = double;

DECLARE_VARIABLE(real, x)
DECLARE_VARIABLE(real, y)

int main() {
  constexpr auto func = 2+(x+x)+7+x+y+4+x;

  std::map<std::string, real> context;
  context["x"] = 5;
  context["y"] = 2;
  std::cout << func(context) << std::endl;

  constexpr real dx = func.gradient<VARIABLE(x)>();
  static_assert(dx==4, "error");

  constexpr real dy = func.gradient<VARIABLE(y)>();
  static_assert(dy==1, "error");
}
