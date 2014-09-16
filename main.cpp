#include <iostream>
#include <type_traits>
#include "operators/add.hpp"

VARIABLE(double, x)
VARIABLE(double, y)

int main() {
  auto func = 2+(x+x)+7+x+y+4+x;
  std::cout << func.gradient(x) << std::endl;
}
