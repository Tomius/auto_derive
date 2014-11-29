#include "all.hpp"
#include "test/assert_equals.hpp"

void functions() {
  // In order to build a metamathematical function you need a variable first.
  // You can define one using the AUTO_DERIVE_VARIABLE(var_type, var_name) macro.
  AUTO_DERIVE_VARIABLE(int, x);
  // x is now a constexpr variable here, representing a metamathematical
  // variable that can hold integer values. It can be used to build up functions.

  // The operators used on x return function objects.
  constexpr auto f = x+2;

  // The function objects can be evaluated using the function call syntax.
  constexpr int int_value = f(1); // evaluates x+2 where x is 1.

  // The result is of course 3.
  STATIC_ASSERT_EQUALS(int_value, 3);

  // The use of auto is highly recommended when declaring functions.
  constexpr auto_derive::Add<auto_derive::Variable<int, struct x>, int> g = f;
}

void multi_variable_functions() {
  AUTO_DERIVE_VARIABLE(double, x);
  AUTO_DERIVE_VARIABLE(double, y);

  // You can use any number of variables in an expression.
  constexpr auto f = (x/2) * y;

  // To evaluate it, you should specify the value
  // of every variable, using the following syntax:
  constexpr double value = f(x=1.5, y=4);

  // The result is 3 again.
  STATIC_ASSERT_EQUALS(value, 3);

  // The order you specify the values for the variable does not matter.
  constexpr double value2 = f(y=4, x=1.5);
  STATIC_ASSERT_EQUALS(value, value2);

  // You can also only partially evaluate a function (like std::bind).
  constexpr auto g = f(x=4);

  // In this case, the result will be g = 2*y.
  // Note that you can evaluate g|y=15 as either g(15) and g(y=15).
  STATIC_ASSERT_EQUALS(g(y=15), 30);

  // The shorter evaluation syntax also works for multi variable functions,
  // then all the variables will have the specified value.
  STATIC_ASSERT_EQUALS(f(5), f(x=5, y=5));
}

void derivation() {
  AUTO_DERIVE_VARIABLE(int, x);
  constexpr auto f = x+2;

  // You can derive an f function according to the x variable using derive(f, x).
  // In this simple case the derivative of x+2 is simply one.
  STATIC_ASSERT_EQUALS(derive(f, x), 1);

  // You can derive the function according to other variables too.
  AUTO_DERIVE_VARIABLE(int, y);
  STATIC_ASSERT_EQUALS(derive(f, y), 0);
}

AUTO_DERIVE_VARIABLE(double, x);
AUTO_DERIVE_VARIABLE(double, y);

void more_about_derivation() {
  constexpr auto f = x*x;

  // The derivative of a function might be a function too.
  constexpr auto dfdx = derive(f, x);

  // The derivative of x*x is 2*x of course.
  STATIC_ASSERT_EQUALS(dfdx(x=42), 84);

  // Since dfdx is a function it can be derived again, and that will result 2.
  STATIC_ASSERT_EQUALS(derive(dfdx, x), 2);
}

void higher_level_derivatives() {
  constexpr auto f = x*x*x;

  // You can calculate the nth derivate using n nested derive() calls.
  STATIC_ASSERT_EQUALS(derive(derive(derive(f, x), x), x), 6);

  // This might be tedious, and hard to read, so
  // there's a "syntactic sugar" for this operation.
  STATIC_ASSERT_EQUALS((derive<3>(f, x)), 6);
  STATIC_ASSERT_EQUALS(derive<50>(f, x), 0);
}

void more_functions() {
  // You can use most of the <cmath> functions on the variables
  // note that f is constexpr even though std::pow isn't.
  constexpr auto f = pow(x, y);

  // Deriving f is constexpr too, and we get y*pow(x, y-1).
  constexpr auto dfdx = derive(f, x);

  // You can print functions out simply with operator<<, to check the result
  std::cout << dfdx << "\n\n";

  // The evaluation of <cmath> functions are done run-time.
  double value = dfdx(x=2, y=3);
  ASSERT_EQUALS(value, 3*pow(2, 3-1));

  // You can build up arbitrarily complex expressions.
  constexpr auto g = atan2(pow(abs(y+x), pow(sqrt(abs(sinh(x)+2)), 2.3)), sin(x)/x);

  // The derivation is still constexpr, but it returns a rather nasty function.
  constexpr auto dgdy = derive(g, y);

  // Try guessing the output :D
  std::cout << dgdy << "\n\n";

  // The list of the supported functions:
  // +, -, *, /, abs, fabs, exp, log, log10, pow, sqrt,
  // sin, cos, tan, sinh, cosh, tanh, asin, acos, atan, atan2
}

// You are also able to write higher-order functions, that
// may take functions as parameters, and also return functions.
template<typename L, typename R>
constexpr auto add_functions(L const& left, R const& right) {
  return left + right;
}

void higher_order_functions() {
  constexpr auto h = add_functions(2*x, x+2);
  STATIC_ASSERT_EQUALS(h(x=5), (2*x + x+2)(x=5));
}

void simplifications() {
  // Printing a function to an ostream tells you exactly
  // which operations will be done when you evaluate it.

  // If you apply a derivation rule to a function the result often holds parts
  // like adding zero or multiplying with one. These parts get optimized out
  // compile-time, so they are not actually evaluated. For ex. the derivative
  // of a pow against two different variables yields two different functions,
  // but they were calculated using the same derivation rule.
  std::cout << derive(pow(x, y), x) << "\n\n"; // pow(x, y-1)*y
  std::cout << derive(pow(x, y), y) << "\n\n"; // pow(x, y)*log(x)
  // These are the results of simplifying the following statement:
  // pow(f, g) * derive(g, v) * log(f) + pow(f, g-1) * g * derive(f, v)

  // However be prepared that the even the simplified forms usually
  // differ from how a human would derive that function.
  // For example derive(x*x*x, x) is x*x+x*(x+x), not 3*x^2
  std::cout << derive(x*x*x, x) << "\n\n";
}

int main() {
  functions();
  multi_variable_functions();
  derivation();
  more_about_derivation();
  higher_level_derivatives();
  more_functions();
  higher_order_functions();
  simplifications();
}
