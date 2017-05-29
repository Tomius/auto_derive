Auto derive
===========

This is a C++14 library offering a way to compile-time derive a mathematical function, so run-time only the derivative is executed. This technique is called
[Automatic differentiation](http://en.wikipedia.org/wiki/Automatic_differentiation), hence the name of the lib.

Example:

```C++
#include "auto_derive/all.hpp"

int main() {
  AUTO_DERIVE_VARIABLE(double, x);
  AUTO_DERIVE_VARIABLE(double, y);

  constexpr auto f = pow(x+y, 3) - 2*x*y;

  constexpr auto dfdy = derive(f, y);

  double dfdy_evaluated = dfdy(x=2, y=4.2);

  std::cout << "derive(" << f << ", " << y << ") = " << dfdy << std::endl;
  std::cout << dfdy << " if x=2 and y=4.2 is " << dfdy_evaluated << std::endl;
}
```

It will result the following output:
``` C++
derive(pow(x+y,3)-2*x*y, y) = 3*pow(x+y,2)-2*x
3*pow(x+y,2)-2*x if x=2 and y=4.2 is 111.32
```

For more details see ```how_to_use.cpp```.

----------------------
If you have any problem, please post in the issues tab or mail me at icyplusplus@gmail.com. Any feedback would be appreciated.
