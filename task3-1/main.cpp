#include <iostream>

#include <stack>

#include "abstract_graph.h"

#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"

template<typename Callback>
void dfs_(const IGraph& graph, std::vector<bool>& visited, int start_vertex, Callback callback) {
  std::stack<int> stack;

  stack.push(start_vertex);

  while (!stack.empty()) {
    int vertex = stack.top();
    stack.pop();

    if (!visited[vertex]) {
      callback(vertex);

      visited[vertex] = true;

      std::vector<int> childs = graph.GetNextVertices(vertex);
      for (auto &child : childs) {
        stack.push(child);
      }
    }
  }
}

template<typename Callback>
void dfs(const IGraph& graph, Callback callback) {
  std::vector<bool> visited;

  visited.resize(graph.VerticesCount());

  for (size_t i = 0; i < graph.VerticesCount(); i++) {
    if (!visited[i]) {
      dfs_(graph, visited, i, callback);
    }
  }
}

int main() {
  ListGraph list_graph(7);

  list_graph.AddEdge(0, 1);
  list_graph.AddEdge(1, 2);
  list_graph.AddEdge(1, 5);
  list_graph.AddEdge(2, 3);
  list_graph.AddEdge(3, 4);
  list_graph.AddEdge(4, 2);

  std::cout << "List graph: " << std::endl;

  dfs(list_graph, [](int v) {
    std::cout << v << std::endl;
  });


  MatrixGraph matrix_graph(list_graph);

  std::cout << "Matrix graph: " << std::endl;

  dfs(matrix_graph, [](int v) {
    std::cout << v << std::endl;
  });

  SetGraph set_graph(matrix_graph);

  std::cout << "Set graph: " << std::endl;

  dfs(set_graph, [](int v) {
    std::cout << v << std::endl;
  });

  return 0;
}
