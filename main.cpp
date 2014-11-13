#include <cmath>
#include <cassert>
#include <iostream>
#include <type_traits>
#include "operators/all.hpp"
#include "derive.hpp"

using real = double;

DECLARE_VARIABLE(real, x);
DECLARE_VARIABLE(real, y);

constexpr double equals(double a, double b) {
  return a-b > 0 ? a-b < 1e-3 : b-a < 1e-3;
}

void test0() {
  DECLARE_VARIABLE(real, x);
  static_assert(!std::is_same<decltype(x), decltype(::y)>::value, "error");
  static_assert(std::is_same<decltype(x), decltype(::x)>::value, "error");
}

void test1() {
  constexpr auto func = 2+(x-x)+7-x+y+4+x;
  static_assert(15 == func(x=5, y=2), "error");

  constexpr auto dx = derive(func, x);
  static_assert(dx==0, "error");
  std::cout << "d/dx(" << func << ") = " << dx << std::endl;

  constexpr real dy = derive(func, y);
  static_assert(dy==1, "error");
  std::cout << "d/dy(" << func << ") = " << dy << std::endl;
}

void test2() {
  constexpr auto func = (2+(x-x)+7-x+y+4+x)*(37-x*y+2+x);
  static_assert(510 == func(x=5, y=2), "error");

  constexpr auto dx = derive(func, x);
  static_assert(-15 == dx(x=5, y=2), "error");
  std::cout << "d/dx(" << func << ") = " << dx << std::endl;

  constexpr auto dy = derive(func, y);
  static_assert(-41 == dy(x=5, y=2), "error");
  std::cout << "d/dy(" << func << ") = " << dy << std::endl;
}

void test3() {
  constexpr auto func = x*x*x;
  static_assert(1*8 == func(x=2), "error");

  constexpr auto dx = derive(func, x);
  static_assert(3*4 == dx(x=2), "error");
  std::cout << "d/dx(" << func << ") = " << dx << std::endl;

  constexpr auto dx2 = derive(dx, x);
  static_assert(6*2 == dx2(x=2), "error");
  std::cout << "d/dx(" << dx << ") = " << dx2 << std::endl;

  constexpr auto dx3 = derive(dx2, x);
  static_assert(dx3 == 6, "error");
  std::cout << "d/dx(" << dx2 << ") = " << dx3 << std::endl;
}

void test4() {
  constexpr auto func = x + derive(y, x);
  constexpr auto func2 = derive(y, x) + x;

  static_assert(std::is_same<decltype(x), decltype(func)>::value, "");
  static_assert(std::is_same<decltype(x), decltype(func2)>::value, "");
}

void test5() {
  constexpr auto func = (13*(x+x)*7-x+y*4*x)*(37-x*y*x+2+y*x);
  static_assert(-945 == func(x=5, y=2), "error");

  constexpr auto dx = derive(func, x);
  static_assert(-17199 == dx(x=5, y=2), "error");
  std::cout << "d/dx(" << func << ") = " << dx << std::endl;

  constexpr auto dy = derive(func, y);
  static_assert(-18920 == dy(x=5, y=2), "error");
  std::cout << "d/dy(" << func << ") = " << dy << std::endl;
}

void test6() {
  constexpr auto func = (13/(x+x)/7-x+y*4/x)*(37-x*y/x+2+y/x);
  static_assert(equals(-1683.0/14.0, func(x=5, y=2)), "error");

  constexpr auto dx = derive(func, x);
  static_assert(equals(-50.5, dx(x=5, y=2)), "error");
  std::cout << "d/dx(" << func << ") = " << dx << std::endl;

  constexpr auto dy = derive(func, y);
  static_assert(equals(5686.0/175.0, dy(x=5, y=2)), "error");
  std::cout << "d/dy(" << func << ") = " << dy << std::endl;
}

void test7() {
  constexpr auto func = ((13*(x+x)*7-x+y*4*x)*(37-x*y*x+2+y*x));
  constexpr auto dx3 = auto_derive::derive<3>(func, x);

  static_assert(-2268 == dx3(x=5, y=2), "error");
  std::cout << "d^3/d^3x(" << func << ") = " << dx3 << std::endl;
}

void test8() {
  constexpr auto func = cos(sin(x));
  assert(equals(1, func(x=M_PI)));

  constexpr auto dx = derive(func, x);
  assert(equals(0, dx(x=M_PI)));
  std::cout << "d/dx(" << func << ") = " << dx << std::endl;
}

void test9() {
  constexpr auto func = pow(x, 2.3);

  constexpr auto dx = derive(func, x);
  assert(equals(5.66326, dx(x=2)));
  std::cout << "d/dx(" << func << ") = " << dx << std::endl;
}

void test10() {
  constexpr auto func = pow(y+x, pow(x+2, 2.3));

  constexpr auto dx = derive(func, x);
  assert(equals(1954.45, dx(x=1, y=0.5)));
  std::cout << "d/dx(" << func << ") = " << dx << std::endl;
}

int main() {
  test1(); std::cout << std::endl;
  test2(); std::cout << std::endl;
  test3(); std::cout << std::endl;
  //test4(); std::cout << std::endl;
  test5(); std::cout << std::endl;
  test6(); std::cout << std::endl;
  test7(); std::cout << std::endl;
  test8(); std::cout << std::endl;
  test9(); std::cout << std::endl;
  test10(); std::cout << std::endl;
}
