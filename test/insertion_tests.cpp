#include <cassert>
#include <sstream>
#include "../all.hpp"

int main() {
  std::stringstream sstream;
  AUTO_DERIVE_VARIABLE(int, x);
  AUTO_DERIVE_VARIABLE(double, y);

  sstream << (
    (x+2-(y/4)) * abs(exp(x)) / log(5*log10(y))
  );
  assert(sstream.str() == "(x+2-y/4)*abs(exp(x))/log(5*log10(y))");

  sstream.str("");
  sstream << (
    atan2(pow(sqrt(1+x), sin(x)), cos(y))
  );
  assert(sstream.str() == "atan2(pow(sqrt(1+x),sin(x)),cos(y))");

  sstream.str("");
  sstream << (
    tan(acos(sinh(x+y)) - asin(atan(cosh(tanh(y*2)))))
  );
  assert(sstream.str() == "tan(acos(sinh(x+y))-asin(atan(cosh(tanh(2*y)))))");

  sstream.str("");
  sstream << (
    asin(cosh(x))-atan(sinh(y))+acos(tanh(x))
  );
  assert(sstream.str() == "asin(cosh(x))-atan(sinh(y))+acos(tanh(x))");

  sstream.str("");
  sstream << (
    ((x+(y-5))/(x+2) + y/x) * (7 + x*3/(1.1*x)/y*2 + -x) / (9 * -(y+0.5))
  );
  assert(sstream.str() == "((x+y-5)/(x+2)+y/x)*(7+2*3*x/(1.1*x)/y+-x)/(9*-(y+0.5))");

  std::cout << "The insertion operator test was successful." << std::endl;
}
