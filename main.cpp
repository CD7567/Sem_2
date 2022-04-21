#include <iostream>
#include <iterator>
#include <vector>
#include "contest_3/part_2/vector/vector.h"

    return best_track_;
  }

 private:
  std::unordered_map<std::string, uint64_t> times_;  //  <user_id, last_time>
  std::unordered_map<uint32_t, int64_t> tracks_;  //  <track_id, track_score>

  std::pair<uint32_t, int64_t> best_track_;
};

void f(int& i) {
  std::cout << "lvalue" << std::endl;
};

void f(int&& i) {
  std::cout << "rvalue" << std::endl;
};

int main() {
  std::allocator<int> allocator;
  int* a = nullptr;

  allocator.deallocate(nullptr, 1);
}