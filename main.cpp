#include <iostream>

template <typename T>
void Swap(T* first, T* second) {
  T tmp = *first;
  *first = *second;
  *second = tmp;
}

template <typename T>
void PushHeap(T* begin, T* end) {
  T* it = end - 1;
  T* ancestor;

  while (begin < it) {
    ancestor = begin + (it - begin - 1) / 2;
    if (*(ancestor) < *it) {
      Swap(ancestor, it);
      it = ancestor;
    } else {
      it = begin;
    }
  }
}

template <typename T>
void PopHeap(T* begin, T* end) {
  Swap(begin, end - 1);
  T* it = begin;
  T* left_child;
  T* right_child = begin + 2;

  while (right_child < end) {
    left_child = begin + (2 * (it - begin) + 1);
    right_child = begin + (2 * (it - begin) + 2);

    if (right_child < end - 1) {
      if (*left_child < *right_child) {
        if (*it < *right_child) {
          Swap(it, right_child);
          it = right_child;
        } else {
          it = end;
        }
      } else {
        if (*it < *left_child) {
          Swap(it, left_child);
          it = left_child;
        } else {
          it = end;
        }
      }
    } else {
      if (left_child < end - 1) {
        if (*it < *left_child) {
          Swap(it, left_child);
          it = left_child;
        } else {
          it = end;
        }
      } else {
        it = end;
      }
    }
  }
}

void Print(int* beg, int* end) {
  for (int* i = beg; i < end; ++i) {
    std::cout << *i << " ";
  }

  std::cout << std::endl;
}

int main() {
  int heap[7]{9, 5, 4, 1, 2, 3, 6};

  PushHeap(heap, heap + 7);

  Print(heap, heap + 7);

  PopHeap(heap, heap + 7);

  Print(heap, heap + 7);
}