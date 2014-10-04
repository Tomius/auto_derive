#include <cmath>
#include <cassert>
#include <iostream>
#include <type_traits>
#include "operators/all.hpp"

using real = double;

namespace variable {
  DECLARE_VARIABLE(real, x);
  DECLARE_VARIABLE(real, y);
}

constexpr double equals(double a, double b) {
  return a-b > 0 ? a-b < 1e-3 : b-a < 1e-3;
}

void test0() {
  using variable::x;
  using variable::y;
  constexpr auto func = 2+(x-x)+7-x+y+4+x;

  static_assert(15 == func(x=5, y=2), "error");

  constexpr auto dx = gradient(func, x);
  static_assert(dx==0, "error");

  constexpr real dy = gradient(func, y);
  static_assert(dy==1, "error");
}

void test1() {
  using variable::x;
  using variable::y;
  constexpr auto func = (2+(x-x)+7-x+y+4+x)*(37-x*y+2+x);
  constexpr auto dx = gradient(func, x);
  constexpr auto dy = gradient(func, y);

  static_assert(510 == func(x=5, y=2), "error");
  static_assert(-15 == dx(x=5, y=2), "error");
  static_assert(-41 == dy(x=5, y=2), "error");
}

void test2() {
  using variable::x;
  constexpr auto func = x*x*x;
  constexpr auto dx = gradient(func, x);
  constexpr auto dx2 = gradient(dx, x);
  constexpr auto dx3 = gradient(dx2, x);

  static_assert(1*8 == func(x=2), "error");
  static_assert(3*4 == dx(x=2), "error");
  static_assert(6*2 == dx2(x=2), "error");
  static_assert(dx3 == 6, "error");
}

void test3() {
  using variable::x;
  using variable::y;
  constexpr auto func = x + gradient(y, x);
  constexpr auto func2 = gradient(y, x) + x;

  static_assert(std::is_same<decltype(x), decltype(func)>::value, "");
  static_assert(std::is_same<decltype(x), decltype(func2)>::value, "");
}

void test4() {
  using variable::x;
  using variable::y;
  constexpr auto func = (13*(x+x)*7-x+y*4*x)*(37-x*y*x+2+y*x);
  constexpr auto dx = gradient(func, x);
  constexpr auto dy = gradient(func, y);

  static_assert(-945 == func(x=5, y=2), "error");
  static_assert(-17199 == dx(x=5, y=2), "error");
  static_assert(-18920 == dy(x=5, y=2), "error");
}

void test5() {
  using variable::x;
  using variable::y;
  constexpr auto func = (13/(x+x)/7-x+y*4/x)*(37-x*y/x+2+y/x);
  constexpr auto dx = gradient(func, x);
  constexpr auto dy = gradient(func, y);

  static_assert(equals(-1683.0/14.0, func(x=5, y=2)), "error");
  static_assert(equals(-50.5, dx(x=5, y=2)), "error");
  static_assert(equals(5686.0/175.0, dy(x=5, y=2)), "error");
}

void test6() {
  using variable::x;
  using variable::y;
  constexpr auto dx3 = gradient(gradient(gradient(
      ((13*(x+x)*7-x+y*4*x)*(37-x*y*x+2+y*x)), x), x), x);

  static_assert(-2268 == dx3(x=5, y=2), "error");
}

void test7() {
  using variable::x;
  constexpr auto func = cos(sin(x));
  constexpr auto dx = gradient(func, x);

  std::cout << "d/dx(" << func << ") = " << dx << std::endl;

  assert(equals(1, func(x=M_PI)));
  assert(equals(0, dx(x=M_PI)));
}

void test8() {
  using variable::x;
  using variable::y;
  constexpr auto func = pow(x, 2.3);
  constexpr auto dx = gradient(func, x);

  std::cout << "d/dx(" << func << ") = " << dx << std::endl;

  assert(equals(5.66326, dx(x=2)));
}

void test9() {
  using variable::x;
  using variable::y;
  constexpr auto func = pow(y+x, pow(x+2, 2.3));
  constexpr auto dx = gradient(func, x);

  std::cout << "d/dx(" << func << ") = " << dx << std::endl;

  assert(equals(1954.45, dx(x=1, y=0.5)));
}

int main() {
  test7();
  test8();
  test9();
}
