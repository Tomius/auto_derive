// Copyright (c) 2014, Tamas Csala

#include <cassert>
#include <iostream>
#include <type_traits>
#include "operators/add.hpp"
#include "operators/sub.hpp"
#include "operators/multiply.hpp"

using real = double;

DECLARE_VARIABLE(real, x)
DECLARE_VARIABLE(real, y)

void test0() {
  constexpr auto func = 2+(x-x)+7-x+y+4+x;

  std::map<std::string, real> context;
  context["x"] = 5;
  context["y"] = 2;
  assert(15 == func(context));

  constexpr real dx = func.gradient<VARIABLE(x)>();
  static_assert(dx==0, "error");

  constexpr real dy = func.gradient<VARIABLE(y)>();
  static_assert(dy==1, "error");
}

void test1() {
  constexpr auto func = (2+(x-x)+7-x+y+4+x)*(37-x*y+2+x);

  std::map<std::string, real> context;
  context["x"] = 5;
  context["y"] = 2;
  assert(510 == func(context));
  assert((-15 == func.gradient<VARIABLE(x)>()(context)));
  assert((-41 == func.gradient<VARIABLE(y)>()(context)));
}

void test2() {
  constexpr auto func = x*x*x;
  constexpr auto dx = func.gradient<VARIABLE(x)>();
  constexpr auto d2x = dx.gradient<VARIABLE(x)>();
  constexpr auto d3x = d2x.gradient<VARIABLE(x)>();

  std::map<std::string, real> context;
  context["x"] = 2;
  assert(1*8 == func(context));
  assert(3*4 == dx(context));
  assert(6*2 == d2x(context));
  static_assert(d3x==6, "error");
}

int main() {
  test0();
  test1();
  test2();
  std::cout << "Test passed without any errors!" << std::endl;
}
