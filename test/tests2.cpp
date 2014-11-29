#include "../all.hpp"
#include "./assert_equals.hpp"

AUTO_DERIVE_VARIABLE(double, x);
AUTO_DERIVE_VARIABLE(double, y);
AUTO_DERIVE_VARIABLE(double, z);
AUTO_DERIVE_VARIABLE(double, w);

/* The test numbers where the functions are tested:
 * operator+ : 0 1 2 5
 * operator- : 0 2
 * operator* : 0 1
 * operator/ : 0 1 4
 * abs, fabs : 0 5
 * exp       : 0
 * log       : 5
 * log10     : 1
 * pow       : 1 5
 * sqrt      : 3 5
 * sin       : 0 1
 * cos       : 1
 * tan       : 1
 * sinh      : 2 5
 * cosh      : 2
 * tanh      : 2 5
 * asin      : 2 5
 * acos      : 2
 * atan      : 2 4
 * atan2     : 4
 */

// http://www.wolframalpha.com/input/?i=evaluate+derive+sin%28exp%28abs%28%28x%2B2-y*z%29%2Fx%29%29%2B7%29+where+x+is+1+y+is+2+z+is+3
void test0() {
  constexpr auto f = sin(exp(abs((x+2-y*z)/x))+7);
  ASSERT_EQUALS(derive(f, x)(x=1, y=2, z=3), 29.9497);
}

// http://www.wolframalpha.com/input/?i=evaluate+derive+sin%28pow%28log10%28x%2Fy%29%2C+z%29%29+%2F+tan%28x%2B+sin%28y%29*cos%28y%29%29+where+x+is+1+y+is+2+z+is+3
void test1() {
  constexpr auto f = sin(pow(log10(x/y), z)) / tan(x+ sin(y)*cos(y));
  ASSERT_EQUALS(derive(f, x)(x=1)(y=2)(z=3), 0.245193);
}

// http://www.wolframalpha.com/input/?i=evaluate+derive+asin%28cosh%28x%29%29-atan%28sinh%28y%29%29%2Bacos%28tanh%28z%29%29+by+y+where+x+is+0.3%2C+y+is+0.5%2C+z+is+0.8
void test2() {
  constexpr auto f = asin(cosh(x))-atan(sinh(y))+acos(tanh(z));
  ASSERT_EQUALS(derive(f, y)(x=0.3, y=0.5, z=0.8), -0.886819);
}

// http://www.wolframalpha.com/input/?i=evaluate+derive+sqrt%28square%28+sqrt%28square%28+sqrt%28square%28x%29%29+%29%29+%29%29+by+x+where+x+is+947.523
void test3() {
  using auto_derive::square;
  constexpr auto f = sqrt(square( sqrt(square( sqrt(square(x)) )) ));
  ASSERT_EQUALS(derive(f, x)(x=947.523), 1);
}

// wolframalpha doesn't know atan2
void test4() {
  using auto_derive::square;
  constexpr auto f = atan(y/x);
  constexpr auto g = atan2(y, x);
  ASSERT_EQUALS(f(x=5.2, y=-17.4), g(x=5.2, y=-17.4));
  ASSERT_EQUALS(derive(f, x)(x=5.2, y=-17.4), derive(g, x)(x=5.2, y=-17.4));
}

// http://www.wolframalpha.com/input/?i=evaluate+derive+%28log%28x%2Bsinh%28y%29-pow%28fabs%28y%29%2C+sqrt%28x%29%29%29%2Bpow%28asin%28z%2F2%29%2C+tanh%28z%29%29%29+by+x+where+x+is+0.3%2C+y+is+0.5%2C+z+is+0.8
void test5() {
  using auto_derive::square;
  constexpr auto f = log(x+sinh(y)-pow(fabs(y), sqrt(x)))+pow(asin(z/2), tanh(z));
  std::cout << derive(f, x)(x=0.3, y=0.5, z=0.8) << std::endl;
  ASSERT_EQUALS(derive(f, x)(x=0.3, y=0.5, z=0.8), 10.4592);
}

int main() {
  test0();
  test1();
  test2();
  test3();
  test4();
  test5();
}
