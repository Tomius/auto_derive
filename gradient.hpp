#ifndef GRADIENT_HPP_
#define GRADIENT_HPP_

namespace auto_derive {

template <typename T, typename U>
constexpr auto gradient(T t, U u) -> decltype(t % u) {
  return t % u;
}

}

#endif
