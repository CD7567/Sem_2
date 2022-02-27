#pragma once

template <typename T>
T Gcd(T x, T y) {
  T result;
  if ((x == 0) || (y == 0)) {
    result = ((x == 0) ? y : x);
  } else {
    x %= y;
    if (x != 0) {
      y %= x;
    }

    result = Gcd(x, y);
  }

  return ((result < 0) ? -result : result);
}