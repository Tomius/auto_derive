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
  template<typename T, typename = void>
  struct HasPrecedence : std::false_type { };

  template<typename T>
  struct HasPrecedence<T, decltype(std::declval<T>().precendence, void())>
      : std::true_type { };

 public:
  PutParenthesisPrinter(std::ostream& os, put_parenthesis context)
      : os_(os), context_(context) {}

  template<typename T>
  std::enable_if_t<HasPrecedence<T>::value, PutParenthesisPrinter const&>
  operator<<(const T& t) const {
    if (t.precendence > context_.precendence) {
      os_ << '(' << t << ')';
    } else {
      os_ << t;
    }

    return *this;
  }

  template<typename T>
  std::enable_if_t<!HasPrecedence<T>::value, PutParenthesisPrinter const&>
  operator<<(const T& t) const {
    os_ << t;
    return *this;
  }

 private:
  std::ostream& os_;
  put_parenthesis context_;
};

PutParenthesisPrinter operator<<(std::ostream& os, put_parenthesis context) {
  return PutParenthesisPrinter(os, context);
}

}

#endif
