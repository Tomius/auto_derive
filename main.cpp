#include <iostream>
#include <type_traits>
#include "operators/add.hpp"

DECLARE_VARIABLE(double, x)
DECLARE_VARIABLE(double, y)

int main() {
  constexpr auto func = 2+(x+x)+7+x+y+4+x;
  constexpr double derivate = func.gradient<VARIABLE(x)>();
  static_assert(derivate==4, "error");

  constexpr double derivate2 = func.gradient<VARIABLE(y)>();
  static_assert(derivate2==1, "error");
}
