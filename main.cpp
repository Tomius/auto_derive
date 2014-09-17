#include <iostream>
#include <type_traits>
#include "operators/add.hpp"

//VARIABLE(double, x)
constexpr char _STRING_OF_x[] = "x";
constexpr Variable<double, _STRING_OF_x> x;

VARIABLE(double, y)

int main() {
  auto func = 2+(x+x)+7+x+y+4+x;
  std::cout << func.gradient<double, _STRING_OF_x, x>() << std::endl;
}
