#include <iostream>
#include <type_traits>
#include "operators/add.hpp"

DECLARE_VARIABLE(real, x)
DECLARE_VARIABLE(real, y)

int main() {
  constexpr auto func = 2+(x+x)+7+x+y+4+x;
  constexpr real dx = func.gradient<VARIABLE(x)>();
  static_assert(dx==4, "error");

  constexpr real dy = func.gradient<VARIABLE(y)>();
  static_assert(dy==1, "error");
}
