#include <iostream>

template <typename T>
T* Partition(T* begin, T* end) {
  T* it_left = begin;
  T* it_right = end;
  T pivot = *(begin + (end - begin) / 2);

  while (it_left <= it_right) {
    while (*it_left < pivot) {
      ++it_left;
    }

    while (pivot < *it_right) {
      --it_right;
    }

    if (it_left < it_right) {
      T* tmp = *it_left;
      *it_left = *it_right;
      *it_right = tmp;

      it_left++;
      it_right--;
    }
  }

  return it_right;
}

template <typename T>
void KthElement(T* begin, T* kth_element, T* end) {
  T* it_left = begin;
  T* it_right = end - 1;

  while (true) {
    T* mid = Partition(it_left, it_right);

    if (kth_element < mid) {
      it_right = mid;
    } else if (mid < kth_element) {
      it_left = mid + 1;
    } else {
      break;
    }
  }
}

int main() {

}