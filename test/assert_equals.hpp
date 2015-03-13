#ifndef TEST_ASSERT_EQUALS_HPP_
#define TEST_ASSERT_EQUALS_HPP_

#include <cmath>
#include <cassert>

// helper to check floating point equalty.
constexpr double kEpsilon = 1e-3;
constexpr bool equals(double a, double b) {
  return a-b > 0 ? a-b < kEpsilon : b-a < kEpsilon;
}

// the <complex> header doesn't compile with libstdc++-4.8
// in C++1y mode, so it is not included by default
#ifdef AUTO_DERIVE_COMPLEX_TESTS
#include <complex>
bool equals(std::complex<double> a, std::complex<double> b) {
  return std::abs(a-b) < kEpsilon;
}
#endif

#define ASSERT_EQUALS(X, Y) assert(equals((X), (Y)))
#define STATIC_ASSERT_EQUALS(X, Y) static_assert(equals((X), (Y)), "")

#endif
