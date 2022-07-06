#include <iostream>
#include <vector>
#include <cstdint>

struct Node {
  int32_t value_ = INT32_MIN;
  int32_t priority_ = INT32_MIN;
  size_t position_ = 0;

  Node* parent_ = nullptr;
  Node* left_child_ = nullptr;
  Node* right_child_ = nullptr;
};

class CartesianTree {
 public:
  using NodePtr = Node*;

  CartesianTree() : root_(nullptr) {
  }
  explicit CartesianTree(const std::vector<std::pair<int32_t, int32_t>>& src) : root_(nullptr) {
    NodePtr last_inserted = nullptr;
    NodePtr curr = nullptr;
    size_t position = 0;

    for (auto& vertex : src) {
      auto new_node = new Node{vertex.first, vertex.second, ++position};
      curr = last_inserted;

      while (curr != nullptr && vertex.second < curr->priority_) {
        curr = curr->parent_;
      }

      if (curr == nullptr) {
        new_node->left_child_ = root_;

        if (root_ != nullptr) {
          root_->parent_ = new_node;
        }

        root_ = new_node;
      } else {
        new_node->left_child_ = curr->right_child_;

        if (curr->right_child_ != nullptr) {
          curr->right_child_->parent_ = new_node;
        }

        curr->right_child_ = new_node;
        new_node->parent_ = curr;
      }

      last_inserted = new_node;
    }
  }

  ~CartesianTree() {
    ReversedTour(root_, [](NodePtr node) { delete node; });
  }

  void Print() {
    InOrderTour(root_, [](NodePtr node) {
      size_t parent_number = node->parent_ == nullptr ? 0 : node->parent_->position_;
      size_t left_child_number = node->left_child_ == nullptr ? 0 : node->left_child_->position_;
      size_t right_child_number = node->right_child_ == nullptr ? 0 : node->right_child_->position_;

      std::cout << parent_number << ' ' << left_child_number << ' ' << right_child_number << '\n';
    });
  }

 private:
  NodePtr root_;

  static void ReversedTour(NodePtr node, void (*fun)(NodePtr)) {
    if (node->left_child_ != nullptr) {
      ReversedTour(node->left_child_, fun);
    }

    if (node->right_child_ != nullptr) {
      ReversedTour(node->right_child_, fun);
    }

    fun(node);
  }
  static void InOrderTour(NodePtr node, void (*fun)(NodePtr)) {
    if (node->left_child_ != nullptr) {
      InOrderTour(node->left_child_, fun);
    }

    fun(node);

    if (node->right_child_ != nullptr) {
      InOrderTour(node->right_child_, fun);
    }
  }
};

int main() {
  size_t size;
  std::cin >> size;

  std::vector<std::pair<int32_t, int32_t>> nodes(size);

  for (size_t i = 0; i < size; ++i) {
    std::cin >> nodes[i].first >> nodes[i].second;
  }

  CartesianTree tree(nodes);

  std::cout << "YES" << '\n';
  tree.Print();
}