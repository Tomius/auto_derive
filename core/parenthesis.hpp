#ifndef PARENTHESIS_HPP_
#define PARENTHESIS_HPP_

// libstdc++-4.8, c++1y mode bug fix
#include <stdio.h>
#undef gets
char *gets(char *p) {
  scanf("%[^\n]%*c", p);
  return p;
}

#include <iostream>
#include <type_traits>

namespace auto_derive {

struct set_precendence {
  int precendence;
  explicit set_precendence(int precendence) : precendence(precendence) {}
};

class PrecedenceAwarePrinter {
 private:
  template<typename T, typename = void>
  struct HasPrecedence : std::false_type { };

  template<typename T>
  struct HasPrecedence<T, decltype(std::declval<T>().precendence,
                                   std::declval<void>())>
      : std::true_type { };

 public:
  PrecedenceAwarePrinter(std::ostream& os, set_precendence context)
      : os_(os), context_(context) {}

  template<typename T>
  std::enable_if_t<HasPrecedence<T>::value, PrecedenceAwarePrinter const&>
  operator<<(const T& t) const {
    if (t.precendence > context_.precendence) {
      os_ << '(' << t << ')';
    } else {
      os_ << t;
    }

    return *this;
  }

  template<typename T>
  std::enable_if_t<!HasPrecedence<T>::value, PrecedenceAwarePrinter const&>
  operator<<(const T& t) const {
    os_ << t;
    return *this;
  }

 private:
  std::ostream& os_;
  set_precendence context_;
};

PrecedenceAwarePrinter operator<<(std::ostream& os, set_precendence context) {
  return PrecedenceAwarePrinter(os, context);
}

}

#endif
