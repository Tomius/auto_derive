#include "all.hpp"
#include "test/assert_equals.hpp"

void functions() {
  // In order to declare functions, you need to declare the variables first,
  // that the function will use. The DECLARE_VARIABLE(type, variable_name) macro
  // creates a constexpr variable named variable_name, that you can operate on.
  DECLARE_VARIABLE(int, x);

  // The operations used on x return function objects (with really long type names)
  constexpr auto_derive::Add<auto_derive::Variable<int, struct x>, int> f = x + 2;

  // The use of auto is highly recommended to maintain readability
  constexpr auto g = x + 2;

  // The function objects can be evaluated using the function call syntax
  constexpr int value = g(1);

  // The result is of course 3
  STATIC_ASSERT_EQUALS(value, 3);
}

void multi_variable_functions() {
  DECLARE_VARIABLE(double, x);
  DECLARE_VARIABLE(double, y);

  // you can use any number of variables in an expression
  constexpr auto f = (x/2) * y;

  // to evaluate it, you should specify the value
  // of every variable, using the following syntax:
  constexpr double value = f(x=1.5, y=4);

  // the result is 3 again
  STATIC_ASSERT_EQUALS(value, 3);

  // the order you specify the values does not matter
  constexpr double value2 = f(y=4, x=1.5);
  STATIC_ASSERT_EQUALS(value, value2);

  // you can also only partially evaluate a function
  constexpr auto g = f(x=4);

  // in this case, it will result the 2*y function.
  // note that you can evaluate it as either g(15) and g(y=15).
  STATIC_ASSERT_EQUALS(g(y=15), 30);

  // the shorter evaluation syntax also works for multi variable functions,
  // then all the variables will have the specified value.
  STATIC_ASSERT_EQUALS(f(5), f(x=5, y=5));
}

void derivation() {
  DECLARE_VARIABLE(int, x);
  constexpr auto f = x + 2;

  // You can derive an f function according to the x variable using derive(f, x).
  // In this simple case the derivative is simply one.
  STATIC_ASSERT_EQUALS(derive(f, x), 1);

  // You can derive the function according to other variables too
  DECLARE_VARIABLE(int, y);
  STATIC_ASSERT_EQUALS(derive(f, y), 0);
}

DECLARE_VARIABLE(double, x);
DECLARE_VARIABLE(double, y);

void more_about_derivation() {
  constexpr auto f = x*x;

  // the derivative of a function might be a function too.
  constexpr auto dfdx = derive(f, x);

  // the derivative is 2*x of course
  STATIC_ASSERT_EQUALS(dfdx(x=42), 84);

  // since dfdx is a function it can be derived again. d(2*x)/dx is 2.
  STATIC_ASSERT_EQUALS(derive(dfdx, x), 2);
}

void higher_level_derivatives() {
  constexpr auto f = x*x*x;

  // you can calculate the nth derivate using n nested derive() calls
  STATIC_ASSERT_EQUALS(derive(derive(derive(f, x), x), x), 6);

  // this might be tedious, and hard to read, so there's a
  // "syntactic sugar" for this operation.
  STATIC_ASSERT_EQUALS(auto_derive::derive<3>(f, x), 6);

  // Unfortunately, the argument dependent lookup won't work for the nth
  // derivative, so you must specify the namespace too. Though a simple using
  // statement can solve this.
  using auto_derive::derive;
  STATIC_ASSERT_EQUALS(derive<3>(f, x), 6);
}

void more_functions() {
  // You can use most of the <cmath> functions on the variables
  constexpr auto f = pow(x, y);

  // deriving f we get y*pow(x, y-1) as expected
  constexpr auto dfdx = derive(f, x);

  // you can print functions out simply with operator<<
  std::cout << dfdx << std::endl;

  // the evaluation is done run-time (calls std::pow, thats not constexpr).
  // but the derivation was done compile-time.
  double value = dfdx(x=2, y=3);
  ASSERT_EQUALS(value, 3*pow(2, 3-1));

  // You can build up arbitrarily complex expressions
  constexpr auto g = atan2(pow(abs(y+x), pow(sqrt(abs(sinh(x)+2)), 2.3)), sin(x)/x);

  // the derivation is still constexpr, but it returns a rather nasty function.
  constexpr auto dgdy = derive(g, y);

  // try guessing the output :D
  std::cout << dgdy << std::endl;

  // The list of the supported functions:
  // +, -, *, /, abs, fabs, log, log10, pow, sqrt, square,
  // sin, cos, tan, sinh, cosh, tanh, asin, acos, atan, atan2
}

int main() {
  functions();
  multi_variable_functions();
  derivation();
  more_about_derivation();
  higher_level_derivatives();
  more_functions();
}
