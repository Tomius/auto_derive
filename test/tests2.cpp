#include "../all.hpp"
#include "./assert_equals.hpp"

AUTO_DERIVE_VARIABLE(double, x);
AUTO_DERIVE_VARIABLE(double, y);
AUTO_DERIVE_VARIABLE(double, z);

/* The test numbers where the functions are tested:
 * operator+ : 0 1 2 5
 * operator- : 0 2 4
 * operator* : 0 1 4
 * operator/ : 0 1 4
 * abs, fabs : 0 5
 * exp       : 0 6
 * log       : 5 6
 * log10     : 1 6
 * pow       : 1 5
 * sqrt      : 3 5
 * sin       : 0 1
 * cos       : 1 6
 * tan       : 1 6
 * sinh      : 2 5
 * cosh      : 2 6
 * tanh      : 2 5
 * asin      : 2 5
 * acos      : 2 6
 * atan      : 2 4
 * atan2     : 4 6
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

// http://www.wolframalpha.com/input/?i=evaluate+atan2%28y-x%2F2%2C+x*y%29+where+x+is+1%2C+and+y+is+2
// http://www.wolframalpha.com/input/?i=evaluate+derivative+of+atan2%28y-x%2F2%2C+x*y%29+by+x+where+x+is+1%2C+and+y+is+2
void test4() {
  constexpr auto f = atan2(y-x/2, x*y);
  constexpr auto g = atan((y-x/2) / (x*y));
  ASSERT_EQUALS(f(x=1, y=2), 0.643501);
  ASSERT_EQUALS(g(x=1, y=2), 0.643501);
  ASSERT_EQUALS(derive(f, x)(x=1, y=2), -0.64);
  ASSERT_EQUALS(derive(g, x)(x=1, y=2), -0.64);
}

// http://www.wolframalpha.com/input/?i=evaluate+derive+%28log%28x%2Bsinh%28y%29-pow%28fabs%28y%29%2C+sqrt%28x%29%29%29%2Bpow%28asin%28z%2F2%29%2C+tanh%28z%29%29%29+by+x+where+x+is+0.3%2C+y+is+0.5%2C+z+is+0.8
void test5() {
  constexpr auto f = log(x+sinh(y)-pow(fabs(y), sqrt(x)))+pow(asin(z/2), tanh(z));
  ASSERT_EQUALS(derive(f, x)(x=0.3, y=0.5, z=0.8), 10.4592);
}

// http://www.wolframalpha.com/input/?i=evaluate+derivative+of+atan2%28exp%28log%28log10%28acos%28sin%28tan%28cosh%28x%29%29%29%29%29%29%29%2C+x%29+by+x+where+x+is+1
void test6() {
 constexpr auto f = atan2(exp(log(log10(acos(sin(tan(cosh(x))))))), x);
 ASSERT_EQUALS(derive(f, x)(x=1), 173.449);
}

int main() {
  test0();
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
}
