#pragma once

template <typename T>
int Partition(T* array_begin, T* array_end) {
  int i = 0;
  int j = array_end - array_begin - 1;
  T pivot_value = array_begin[(i + j) / 2];

  do {
    while (array_begin[i] < pivot_value) {
      i++;
    }

    while (pivot_value < array_begin[j]) {
      j--;
    }

    if (i <= j) {
      T tmp = array_begin[i];
      array_begin[i] = array_begin[j];
      array_begin[j] = tmp;

      i++;
      j--;
    }
  } while (i <= j);

  return i - 1;
}

template <typename T>
void Sort(T* array_begin, T* array_end) {
  int pivot = Partition(array_begin, array_end);

  if (array_begin >= array_end - 1) {
    return;
  }

  Sort(array_begin, array_begin + pivot + 1);
  Sort(array_begin + pivot + 1, array_end);
}