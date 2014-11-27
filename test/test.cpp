#include <cassert>
#include <iostream>
#include <type_traits>
#include "../all.hpp"
#include "./assert_equals.hpp"

DECLARE_VARIABLE(double, x);
DECLARE_VARIABLE(double, y);

void test0() {
  DECLARE_VARIABLE(double, x);
  static_assert(!std::is_same<decltype(x), decltype(::y)>::value, "error");
  static_assert(std::is_same<decltype(x), decltype(::x)>::value, "error");
}

void test1() {
  constexpr auto f = 2+(x-x)+7-x+y+4+x;
  STATIC_ASSERT_EQUALS(f(x=5, y=2), 15);

  constexpr auto dx = derive(f, x);
  STATIC_ASSERT_EQUALS(dx, 0);
  std::cout << "d/dx(" << f << ") = " << dx << std::endl;

  constexpr double dy = derive(f, y);
  STATIC_ASSERT_EQUALS(dy, 1);
  std::cout << "d/dy(" << f << ") = " << dy << std::endl;
}

void test2() {
  constexpr auto f = (2+(x-x)+7-x+y+4+x)*(37-x*y+2+x);
  STATIC_ASSERT_EQUALS(f(x=5, y=2), 510);

  constexpr auto dx = derive(f, x);
  STATIC_ASSERT_EQUALS(dx(x=5, y=2), -15);
  std::cout << "d/dx(" << f << ") = " << dx << std::endl;

  constexpr auto dy = derive(f, y);
  STATIC_ASSERT_EQUALS(dy(x=5, y=2), -41);
  std::cout << "d/dy(" << f << ") = " << dy << std::endl;
}

void test3() {
  constexpr auto f = x*x*x;
  STATIC_ASSERT_EQUALS(f(2), 1*8);

  constexpr auto dx = derive(f, x);
  STATIC_ASSERT_EQUALS(dx(2), 3*4);
  std::cout << "d/dx(" << f << ") = " << dx << std::endl;

  constexpr auto dx2 = derive(dx, x);
  STATIC_ASSERT_EQUALS(dx2(2), 6*2);
  std::cout << "d/dx(" << dx << ") = " << dx2 << std::endl;

  constexpr auto dx3 = derive(dx2, x);
  STATIC_ASSERT_EQUALS(dx3, 6*1);
  std::cout << "d/dx(" << dx2 << ") = " << dx3 << std::endl;
}

void test4() {
  constexpr auto f = x + derive(y, x);
  constexpr auto f2 = derive(y, x) + x;

  static_assert(std::is_same<decltype(x), decltype(f)>::value, "");
  static_assert(std::is_same<decltype(x), decltype(f2)>::value, "");
}

void test5() {
  constexpr auto f = (13*(x+x)*7-x+y*4*x)*(37-x*y*x+2+y*x);
  STATIC_ASSERT_EQUALS(f(x=5, y=2), -945);

  constexpr auto dx = derive(f, x);
  STATIC_ASSERT_EQUALS(dx(x=5, y=2), -17199);
  std::cout << "d/dx(" << f << ") = " << dx << std::endl;

  constexpr auto dy = derive(f, y);
  STATIC_ASSERT_EQUALS(dy(x=5, y=2), -18920);
  std::cout << "d/dy(" << f << ") = " << dy << std::endl;
}

void test6() {
  constexpr auto f = (13/(x+x)/7-x+y*4/x)*(37-x*y/x+2+y/x);
  STATIC_ASSERT_EQUALS(f(x=5, y=2), -1683.0/14.0);

  constexpr auto dx = derive(f, x);
  STATIC_ASSERT_EQUALS(dx(x=5, y=2), -50.5);
  std::cout << "d/dx(" << f << ") = " << dx << std::endl;

  constexpr auto dy = derive(f, y);
  STATIC_ASSERT_EQUALS(dy(x=5, y=2), 5686.0/175.0);
  std::cout << "d/dy(" << f << ") = " << dy << std::endl;
}

void test7() {
  constexpr auto f = ((13*(x+x)*7-x+y*4*x)*(37-x*y*x+2+y*x));
  constexpr auto dx3 = auto_derive::derive<3>(f, x);

  STATIC_ASSERT_EQUALS(dx3(x=5, y=2), -2268);
  std::cout << "d^3/d^3x(" << f << ") = " << dx3 << std::endl;
}

void test8() {
  constexpr auto f = cos(sin(x));
  ASSERT_EQUALS(f(M_PI), 1);

  constexpr auto dx = derive(f, x);
  ASSERT_EQUALS(dx(M_PI), 0);
  std::cout << "d/dx(" << f << ") = " << dx << std::endl;
}

void test9() {
  constexpr auto f = pow(x, 2.3);

  constexpr auto dx = derive(f, x);
  ASSERT_EQUALS(dx(2), 5.66326);
  std::cout << "d/dx(" << f << ") = " << dx << std::endl;
}

void test10() {
  constexpr auto f = pow(y+x, pow(x+2, 2.3));

  constexpr auto dx = derive(f, x);
  ASSERT_EQUALS(dx(x=1, y=0.5), 1954.45);
  std::cout << "d/dx(" << f << ") = " << dx << std::endl;
}

int main() {
  test1(); std::cout << std::endl;
  test2(); std::cout << std::endl;
  test3(); std::cout << std::endl;
  test5(); std::cout << std::endl;
  test6(); std::cout << std::endl;
  test7(); std::cout << std::endl;
  test8(); std::cout << std::endl;
  test9(); std::cout << std::endl;
  test10(); std::cout << std::endl;
}
