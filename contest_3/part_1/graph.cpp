#include <iostream>
#include <vector>
#include <algorithm>

struct Vertex {
  std::vector<size_t> adjacent_;
};

class Graph {
 public:
  explicit Graph(size_t dim) : matrix_(std::vector<Vertex>(dim + 1, Vertex())){}

  void AddEdge(size_t u, size_t v) {
    matrix_[u].adjacent_.push_back(v);
    matrix_[v].adjacent_.push_back(u);
  }

  const std::vector<size_t>& VertexFun(size_t u) {
    return matrix_[u].adjacent_;
  }

 private:
  std::vector<Vertex> matrix_;
};

int main() {
  size_t commands_amount, graph_size, u, v;
  int64_t command;

  std::cin >> graph_size >> commands_amount;

  Graph graph(graph_size);

  for (size_t i = 0; i < commands_amount; ++i) {
    std::cin >> command;

    if (command == 1) {
      std::cin >> u >> v;
      graph.AddEdge(u, v);
    } else {
      std::cin >> u;
      const std::vector<size_t>& adjacent = graph.VertexFun(u);

      for (auto& elem : adjacent) {
        std::cout << elem << " ";
      }
      std::cout << std::endl;
    }
  }
}