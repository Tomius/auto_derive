#include <complex>
#include "../all.hpp"
#include "./assert_equals.hpp"

AUTO_DERIVE_VARIABLE(double, x);
AUTO_DERIVE_VARIABLE(double, y);
AUTO_DERIVE_VARIABLE(double, z);

/* The test numbers where the specific functions are tested:
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

void test0() {
  constexpr auto f = sin(exp(abs((x+2-y*z)/x))+7);
  // http://www.wolframalpha.com/input/?i=evaluate+sin%28exp%28abs%28%28x%2B2-y*z%29%2Fx%29%29%2B7%29+where+x+is+1+y+is+2+z+is+3
  ASSERT_EQUALS(f(x=1, y=2, z=3), 0.927921);
  // http://www.wolframalpha.com/input/?i=evaluate+derive+sin%28exp%28abs%28%28x%2B2-y*z%29%2Fx%29%29%2B7%29+where+x+is+1+y+is+2+z+is+3
  ASSERT_EQUALS(derive(f, x)(x=1, y=2, z=3), 29.9497);
}

void test1() {
  constexpr auto f = sin(pow(log10(x/y), z)) / tan(x+ sin(y)*cos(y));
  // http://www.wolframalpha.com/input/?i=evaluate+sin%28pow%28log10%28x%2Fy%29%2C+z%29%29+%2F+tan%28x%2B+sin%28y%29*cos%28y%29%29+where+x+is+1+y+is+2+z+is+3
  ASSERT_EQUALS(f(x=1)(y=2)(z=3), -0.0380772);
  // http://www.wolframalpha.com/input/?i=evaluate+derive+sin%28pow%28log10%28x%2Fy%29%2C+z%29%29+%2F+tan%28x%2B+sin%28y%29*cos%28y%29%29+where+x+is+1+y+is+2+z+is+3
  ASSERT_EQUALS(derive(f, x)(x=1)(y=2)(z=3), 0.245193);
}

void test2() {
  AUTO_DERIVE_VARIABLE(std::complex<double>, x);
  AUTO_DERIVE_VARIABLE(std::complex<double>, y);
  AUTO_DERIVE_VARIABLE(std::complex<double>, z);
  constexpr auto f = asin(cosh(x))-atan(sinh(y))+acos(tanh(z));
  // http://www.wolframalpha.com/input/?i=evaluate+asin%28cosh%28x%29%29-atan%28sinh%28y%29%29%2Bacos%28tanh%28z%29%29+where+x+is+0.3%2C+y+is+0.5%2C+z+is+0.8
  ASSERT_EQUALS(f(x=0.3, y=0.5, z=0.8), (std::complex<double>{1.93501, 0.3}));
  // http://www.wolframalpha.com/input/?i=evaluate+derive+asin%28cosh%28x%29%29-atan%28sinh%28y%29%29%2Bacos%28tanh%28z%29%29+by+y+where+x+is+0.3%2C+y+is+0.5%2C+z+is+0.8
  ASSERT_EQUALS(derive(f, y)(x=0.3, y=0.5, z=0.8), -0.886819);
}

void test3() {
  using auto_derive::square;
  constexpr auto f = sqrt(square( sqrt(square( sqrt(square(x)) )) ));
  // http://www.wolframalpha.com/input/?i=evaluate+sqrt%28square%28+sqrt%28square%28+sqrt%28square%28x%29%29+%29%29+%29%29+where+x+is+947.523
  ASSERT_EQUALS(f(x=947.523), 947.523);
  // http://www.wolframalpha.com/input/?i=evaluate+derive+sqrt%28square%28+sqrt%28square%28+sqrt%28square%28x%29%29+%29%29+%29%29+by+x+where+x+is+947.523
  ASSERT_EQUALS(derive(f, x)(x=947.523), 1);
}

void test4() {
  constexpr auto f = atan2(y-x/2, x*y);
  // http://www.wolframalpha.com/input/?i=evaluate+atan2%28y-x%2F2%2C+x*y%29+where+x+is+1%2C+and+y+is+2
  ASSERT_EQUALS(f(x=1, y=2), 0.643501);
  // http://www.wolframalpha.com/input/?i=evaluate+derivative+of+atan2%28y-x%2F2%2C+x*y%29+by+x+where+x+is+1%2C+and+y+is+2
  ASSERT_EQUALS(derive(f, x)(x=1, y=2), -0.64);

  constexpr auto g = atan((y-x/2) / (x*y));
  // http://www.wolframalpha.com/input/?i=evaluate+atan%28%28y-x%2F2%29+%2F+%28x*y%29%29+where+x+is+1%2C+and+y+is+2
  ASSERT_EQUALS(g(x=1, y=2), 0.643501);
  // http://www.wolframalpha.com/input/?i=evaluate+derivative+of+atan%28%28y-x%2F2%29+%2F+%28x*y%29%29+by+x+where+x+is+1%2C+and+y+is+2
  ASSERT_EQUALS(derive(g, x)(x=1, y=2), -0.64);
}

void test5() {
  constexpr auto f = log(x+sinh(y)-pow(fabs(y), sqrt(x)))+pow(asin(z/2), tanh(z));
  // http://www.wolframalpha.com/input/?i=evaluate+%28log%28x%2Bsinh%28y%29-pow%28fabs%28y%29%2C+sqrt%28x%29%29%29%2Bpow%28asin%28z%2F2%29%2C+tanh%28z%29%29%29+where+x+is+0.3%2C+y+is+0.5%2C+z+is+0.8
  ASSERT_EQUALS(f(x=0.3, y=0.5, z=0.8), -1.43325);
  // http://www.wolframalpha.com/input/?i=evaluate+derive+%28log%28x%2Bsinh%28y%29-pow%28fabs%28y%29%2C+sqrt%28x%29%29%29%2Bpow%28asin%28z%2F2%29%2C+tanh%28z%29%29%29+by+x+where+x+is+0.3%2C+y+is+0.5%2C+z+is+0.8
  ASSERT_EQUALS(derive(f, x)(x=0.3, y=0.5, z=0.8), 10.4592);
}

void test6() {
  constexpr auto f = atan2(exp(log(log10(acos(sin(tan(cosh(x))))))), x);
  // http://www.wolframalpha.com/input/?i=evaluate+atan2%28exp%28log%28log10%28acos%28sin%28tan%28cosh%28x%29%29%29%29%29%29%29%2C+x%29+where+x+is+1
  ASSERT_EQUALS(f(x=1), 0.454979);
  // http://www.wolframalpha.com/input/?i=evaluate+derivative+of+atan2%28exp%28log%28log10%28acos%28sin%28tan%28cosh%28x%29%29%29%29%29%29%29%2C+x%29+by+x+where+x+is+1
  ASSERT_EQUALS(derive(f, x)(x=1), 173.449);
}

void test7() {
  constexpr auto f = (x+y)/(x-y);
  // http://www5b.wolframalpha.com/input/?i=evaluate+%28x%2By%29%2F%28x-y%29+where+x+is+5+and+y+is+12
  ASSERT_EQUALS(derive<0>(f, x)(x=5, y=12), -17.0/7.0);
  // http://www5b.wolframalpha.com/input/?i=evaluate+derivative+of+%28x%2By%29%2F%28x-y%29+by+x+where+x+is+5+and+y+is+12
  ASSERT_EQUALS(derive<1>(f, x)(x=5, y=12), -24.0/49.0);
  // http://www5b.wolframalpha.com/input/?i=evaluate+second+derivative+of+%28x%2By%29%2F%28x-y%29+where+x+is+5+and+y+is+12
  ASSERT_EQUALS(derive<2>(f, x)(x=5, y=12), -48.0/343.0);
  // http://www5b.wolframalpha.com/input/?i=evaluate+third+derivative+of+%28x%2By%29%2F%28x-y%29+where+x+is+5+and+y+is+12
  ASSERT_EQUALS(derive<3>(f, x)(x=5, y=12), -144.0/2401.0);
  // http://www5b.wolframalpha.com/input/?i=evaluate+fourth+derivative+of+%28x%2By%29%2F%28x-y%29+where+x+is+5+and+y+is+12
  ASSERT_EQUALS(derive<4>(f, x)(x=5, y=12), -576.0/16807.0);
}

int main() {
  test0(); test1(); test2(); test3();
  test4(); test5(); test6(); test7();
  std::cout << "All of the tests were successful." << std::endl;
}
