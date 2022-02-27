#pragma once

template <typename T, typename U, typename V>
void Merge(const T* first_begin, const T* first_end, const U* second_begin, const U* second_end, V* out) {
  const T* it_first = first_begin;
  const U* it_second = second_begin;
  V* it_out = out;

  while ((it_first < first_end) || (it_second < second_end)) {
    if (it_first < first_end) {
      if (it_second < second_end) {
        if (*it_first < *it_second) {
          *it_out = *it_first;
          it_first++;
        } else {
          *it_out = *it_second;
          it_second++;
        }
      } else {
        *it_out = *it_first;
        it_first++;
      }
    } else {
      *it_out = *it_second;
      it_second++;
    }

    it_out++;
  }
}