#include <cmath>
#include <cassert>
#include <iostream>
#include <typeinfo>
#include <type_traits>
#include "operators/all.hpp"

using real = double;

namespace variable {
DECLARE_VARIABLE(real, x)
DECLARE_VARIABLE(real, y)
}

#define assertEquals(a, b) assert(std::fabs((a)-(b)) < 1e-5)

void test0() {
  using variable::x;
  using variable::y;
  constexpr auto func = 2+(x-x)+7-x+y+4+x;

  std::map<std::string, real> context;
  context["x"] = 5;
  context["y"] = 2;
  assertEquals(15, func(context));

  constexpr auto dx = func.gradient(x);
  static_assert(dx==0, "error");

  constexpr real dy = func.gradient(y);
  static_assert(dy==1, "error");
}

void test1() {
  using variable::x;
  using variable::y;
  constexpr auto func = (2+(x-x)+7-x+y+4+x)*(37-x*y+2+x);
  constexpr auto dx = func.gradient(x);
  constexpr auto dy = func.gradient(y);

  std::map<std::string, real> context;
  context["x"] = 5;
  context["y"] = 2;
  assertEquals(510, func(context));
  assertEquals(-15, dx(context));
  assertEquals(-41, dy(context));
}

void test2() {
  using variable::x;
  constexpr auto func = x*x*x;
  constexpr auto dx = func.gradient(x);
  constexpr auto dx2 = dx.gradient(x);
  constexpr auto dx3 = dx2.gradient(x);

  std::map<std::string, real> context;
  context["x"] = 2;
  assertEquals(1*8, func(context));
  assertEquals(3*4, dx(context));
  assertEquals(6*2, dx2(context));
  static_assert(dx3==6, "error");
}

void test3() {
  using variable::x;
  using variable::y;
  constexpr auto func = x + y.gradient(x);
  constexpr auto func2 = y.gradient(x) + x;
  static_assert(std::is_same<decltype(x), decltype(func)>::value, "");
  static_assert(std::is_same<decltype(x), decltype(func2)>::value, "");
}

void test4() {
  using variable::x;
  using variable::y;
  constexpr auto func = (13*(x+x)*7-x+y*4*x)*(37-x*y*x+2+y*x);
  constexpr auto dx = func.gradient(x);
  constexpr auto dy = func.gradient(y);

  std::map<std::string, real> context;
  context["x"] = 5;
  context["y"] = 2;

  assertEquals(-945, func(context));
  assertEquals(-17199, dx(context));
  assertEquals(-18920, dy(context));
}

void test5() {
  using variable::x;
  using variable::y;
  constexpr auto func = (13/(x+x)/7-x+y*4/x)*(37-x*y/x+2+y/x);
  constexpr auto dx = func.gradient(x);
  constexpr auto dy = func.gradient(y);

  std::map<std::string, real> context;
  context["x"] = 5;
  context["y"] = 2;

  assertEquals(-1683.0/14.0, func(context));
  assertEquals(-50.5, dx(context));
  assertEquals(5686.0/175.0, dy(context));
}

void test6() {
  using variable::x;
  using variable::y;
  constexpr auto dx3 = ((13*(x+x)*7-x+y*4*x)*(37-x*y*x+2+y*x))
                        .gradient(x)
                        .gradient(x)
                        .gradient(x);

  std::map<std::string, real> context;
  context["x"] = 5;
  context["y"] = 2;

  assertEquals(-2268, dx3(context));
}


int main() {
  test0();
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  std::cout << "Test passed without any errors!" << std::endl;
}
