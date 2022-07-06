#include <iostream>
#include <vector>

class BinaryIndexedTree {
 public:
  explicit BinaryIndexedTree(size_t size) : size_(size), tree_(size, std::vector(size, std::vector<int32_t>(size, 0))) {
  }

  void Update(size_t x, size_t y, size_t z, int32_t delta_elem) {
    for (size_t i = x; i < size_; i |= (i + 1)) {
      for (size_t j = y; j < size_; j |= (j + 1)) {
        for (size_t k = z; k < size_; k |= (k + 1)) {
          tree_[i][j][k] += delta_elem;
        }
      }
    }
  }

  int32_t Sum(size_t beg_x, size_t beg_y, size_t beg_z, size_t end_x, size_t end_y, size_t end_z) {
    int32_t sum = RectSum(end_x, end_y, end_z);

    sum -= RectSum(beg_x - 1, end_y, end_z);
    sum -= RectSum(end_x, beg_y - 1, end_z);
    sum -= RectSum(end_x, end_y, beg_z - 1);

    sum += RectSum(beg_x - 1, beg_y - 1, end_z);
    sum += RectSum(beg_x - 1, end_y, beg_z - 1);
    sum += RectSum(end_x, beg_y - 1, beg_z - 1);

    sum -= RectSum(beg_x - 1, beg_y - 1, beg_z - 1);

    return sum;
  }

 private:
  size_t size_;
  std::vector<std::vector<std::vector<int32_t>>> tree_;

  static inline size_t F(size_t i) {
    return (i & (i + 1));
  }

  int32_t RectSum(size_t x, size_t y, size_t z) {
    int32_t sum = 0;

    if (!(x >= size_ || y >= size_ || z >= size_)) {
      for (size_t i = x; i <= x; i = F(i) - 1) {
        for (size_t j = y; j <= y; j = F(j) - 1) {
          for (size_t k = z; k <= z; k = F(k) - 1) {
            sum += tree_[i][j][k];
          }
        }
      }
    }

    return sum;
  }
};

int main() {
  size_t size;
  size_t x_1, y_1, z_1, x_2, y_2, z_2;
  int32_t delta;
  size_t command;

  std::cin >> size;
  BinaryIndexedTree tree(size);

  do {
    std::cin >> command;

    switch (command) {
      case 1:
        std::cin >> x_1 >> y_1 >> z_1 >> delta;
        tree.Update(x_1, y_1, z_1, delta);
        break;

      case 2:
        std::cin >> x_1 >> y_1 >> z_1 >> x_2 >> y_2 >> z_2;
        std::cout << tree.Sum(x_1, y_1, z_1, x_2, y_2, z_2) << '\n';
        break;
    }
  } while (command != 3);
}