#ifndef PARENTHESIS_HPP_
#define PARENTHESIS_HPP_

#include <iostream>
#include <type_traits>

namespace auto_derive {

struct put_parenthesis {
  int precendence;
  explicit put_parenthesis(int precendence)
      : precendence(precendence) {}
};

class PutParenthesisPrinter {
private:

  template <typename T>
  class HasPrecedence {
    private:
      template <typename C> static char test( decltype(&C::precendence) ) ;
      template <typename C> static long long test(...);

    public:
      enum { value = sizeof(test<T>(0)) == sizeof(char) };
  };

  std::ostream& os;
  put_parenthesis op;

public:
  PutParenthesisPrinter(std::ostream& os, put_parenthesis op)
      : os(os), op(op) { }

  template<typename T>
  std::enable_if_t<HasPrecedence<T>::value, std::ostream&>
  operator<<(const T& t) const {
    if (t.precendence() > op.precendence) {
      return os << '(' << t << ')';
    } else {
      return os << t;
    }
  }

  template<typename T>
  std::enable_if_t<!HasPrecedence<T>::value, std::ostream&>
  operator<<(const T& t) const {
    return os << t;
  }

};

PutParenthesisPrinter operator<<(std::ostream& os, put_parenthesis op) {
  return PutParenthesisPrinter(os, op);
}

}

#endif
