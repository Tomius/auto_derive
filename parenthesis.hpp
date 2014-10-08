#ifndef PARENTHESIS_HPP_
#define PARENTHESIS_HPP_

#include <iostream>
#include <type_traits>

namespace auto_derive {

struct put_parenthesis {
  int precendence;
  explicit put_parenthesis(int precendence) : precendence(precendence) {}
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
      : os(os), op(op) {}

  template<typename T>
  std::enable_if_t<HasPrecedence<T>::value, PutParenthesisPrinter const&>
  operator<<(const T& t) const {
    if (t.precendence() > op.precendence) {
      os << '(' << t << ')';
    } else {
      os << t;
    }

    return *this;
  }

  template<typename T>
  std::enable_if_t<!HasPrecedence<T>::value, PutParenthesisPrinter const&>
  operator<<(const T& t) const {
    os << t;
    return *this;
  }

};

PutParenthesisPrinter operator<<(std::ostream& os, put_parenthesis op) {
  return PutParenthesisPrinter(os, op);
}

}

#endif
