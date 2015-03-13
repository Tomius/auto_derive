Auto derive
===========

This is C++14 library offering a way to compile-time derive a mathematical function, so run-time only the derivative is exectued. This technique is called
[Automatic differentiation](http://en.wikipedia.org/wiki/Automatic_differentiation), hence the name of the lib.

Example:

```C++
#include "auto_derive/all.hpp"

int main() {
  AUTO_DERIVE_VARIABLE(double, x);
  AUTO_DERIVE_VARIABLE(double, y);

  constexpr auto f = atan2(pow(abs(y+x), pow(sqrt(abs(sinh(x)+2)), 2.3)), sin(x)/x);

  constexpr auto dgdy = derive(g, y);

  double eval = dgdy(x=2, y=4.2);

  std::cout << "derive(" << g << ", " << y << ") = " << dgdy << std::endl;
}
```

It will results this output: ```
derive(atan2(pow(abs(y+x),pow(sqrt(abs(sinh(x)+2)),2.3)),sin(x)/x), y) = pow(
abs(y+x),pow(sqrt(abs(sinh(x)+2)),2.3)-1)*pow(sqrt(abs(sinh(x)+2)),2.3)*(y+x)/
abs(y+x)*sin(x)/x/(sin(x)/x*sin(x)/x)/(pow(abs(y+x),pow(sqrt(abs(sinh(x)+2)),
2.3))/(sin(x)/x)*pow(abs(y+x),pow(sqrt(abs(sinh(x)+2)),2.3))/(sin(x)/x)+1)
```

For more details see ```how_to_use.cpp```.

----------------------
If you have any problem, please post in the issues tab or mail me at icyplusplus@gmail.com. Any feedback would be appreciated.
