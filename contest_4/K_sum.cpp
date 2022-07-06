#include <iostream>
#include <vector>
#include <random>
#include <cstdint>

struct Node {
  int64_t value_ = INT64_MIN;
  int64_t priority_ = rand();
  int64_t tree_sum_ = 0;

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

    for (auto& vertex : src) {
      auto new_node = new Node{vertex.first, vertex.second};
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
    Delete(root_);
  }

  NodePtr Find(int64_t value) {
    return Find(root_, value);
  }

  NodePtr Next(int64_t value) {
    return Next(root_, value);
  }
  NodePtr Prev(int64_t value) {
    return Prev(root_, value);
  }
  NodePtr Kth(size_t k) {
    return Kth(root_, k);
  }

  void Insert(int64_t value) {
    if (!Exists(value)) {
      auto split = Split(root_, value);

      root_ = Merge(Merge(split.first, new Node{value}), split.second);
    }
  }
  void Delete(int64_t value) {
    if (Exists(value)) {
      auto split = Split(root_, value);
      auto split_second = Split(split.second, value + 1);

      root_ = Merge(split.first, split_second.second);

      delete split_second.first;
    }
  }

  bool Exists(int64_t value) {
    return Find(value) != nullptr;
  }

  int64_t Sum(int64_t left_bound, int64_t right_bound) {
    int64_t result;

    auto split_1 = Split(root_, left_bound);
    auto less_l_subtree = split_1.first;
    auto geq_l_subtree = split_1.second;

    auto split_2 = Split(geq_l_subtree, right_bound + 1);
    auto less_r_subtree = split_2.first;
    auto geq_r_subtree = split_2.second;

    result = TreeSum(less_r_subtree);
    root_ = Merge(Merge(less_l_subtree, less_r_subtree), geq_r_subtree);

    return result;
  }

 private:
  NodePtr root_;

  static int64_t TreeSum(NodePtr node) {
    return (node == nullptr ? 0 : node->tree_sum_);
  }
  static void FixNode(NodePtr node) {
    if (node != nullptr) {
      node->tree_sum_ = TreeSum(node->left_child_) + TreeSum(node->right_child_) + node->value_;
    }
  }

  static NodePtr Merge(NodePtr lhs_root, NodePtr rhs_root) {
    NodePtr result;

    if (lhs_root == nullptr) {
      result = rhs_root;
    } else if (rhs_root == nullptr) {
      result = lhs_root;
    } else {
      if (lhs_root->priority_ < rhs_root->priority_) {
        result = lhs_root;
        result->right_child_ = Merge(result->right_child_, rhs_root);

        if (result->right_child_ != nullptr) {
          result->right_child_->parent_ = result;
        }
      } else {
        result = rhs_root;
        result->left_child_ = Merge(lhs_root, result->left_child_);

        if (result->left_child_ != nullptr) {
          result->left_child_->parent_ = result;
        }
      }
    }

    FixNode(result);

    return result;
  }
  static std::pair<NodePtr, NodePtr> Split(NodePtr root, int64_t value) {
    std::pair<NodePtr, NodePtr> result;

    if (root == nullptr) {
      result = {nullptr, nullptr};
    } else {
      if (root->value_ < value) {
        auto split = Split(root->right_child_, value);

        root->right_child_ = split.first;

        if (split.first != nullptr) {
          split.first->parent_ = root;
        }

        if (split.second != nullptr) {
          split.second->parent_ = nullptr;
        }

        result = {root, split.second};
      } else {
        auto split = Split(root->left_child_, value);

        root->left_child_ = split.second;

        if (split.first != nullptr) {
          split.first->parent_ = nullptr;
        }

        if (split.second != nullptr) {
          split.second->parent_ = root;
        }

        result = {split.first, root};
      }
    }

    FixNode(result.first);
    FixNode(result.second);

    return result;
  }

  static NodePtr Find(NodePtr root, int64_t value) {
    NodePtr result = nullptr;

    if (root != nullptr) {
      if (root->value_ == value) {
        result = root;
      } else if (root->value_ < value) {
        result = Find(root->right_child_, value);
      } else {
        result = Find(root->left_child_, value);
      }
    }

    return result;
  }
  static NodePtr Next(NodePtr root, int64_t value) {
    NodePtr result = nullptr;

    if (root != nullptr) {
      if (root->value_ > value) {
        result = Next(root->left_child_, value);

        if (result == nullptr) {
          result = root;
        }
      } else if (root->value_ < value) {
        result = Next(root->right_child_, value);
      }
    }

    return result;
  }
  static NodePtr Prev(NodePtr root, int64_t value) {
    NodePtr result = nullptr;

    if (root != nullptr) {
      if (root->value_ < value) {
        result = Prev(root->right_child_, value);

        if (result == nullptr) {
          result = root;
        }
      } else if (root->value_ > value) {
        result = Prev(root->left_child_, value);
      }
    }

    return result;
  }
  static NodePtr Kth(NodePtr root, size_t k) {
    NodePtr result = root;

    while (result != nullptr) {
      size_t left_tree_size = TreeSum(result->left_child_);

      if (left_tree_size < k) {
        k -= left_tree_size + 1;
        result = result->right_child_;
      } else if (left_tree_size > k) {
        result = result->left_child_;
      } else {
        break;
      }
    }

    return result;
  }

  static void Delete(NodePtr node) {
    if (node != nullptr) {
      Delete(node->left_child_);
      Delete(node->right_child_);
    }

    delete node;
  }
};

int main() {
  size_t comm_amount;
  std::cin >> comm_amount;
  auto tree = CartesianTree();

  int64_t value;
  int64_t left_bound, right_bound;
  char command;

  for (size_t i = 0; i < comm_amount; ++i) {
    std::cin >> command;

    if (command == '+') {
      std::cin >> value;
      tree.Insert(value);
    } else if (command == '?') {
      std::cin >> left_bound >> right_bound;
      std::cout << tree.Sum(left_bound, right_bound) << std::endl;
    }
  }
}