#include <iostream>

#include <stack>

#include "abstract_graph.h"

#include "list_graph.h"

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
  ListGraph graph(7);

  graph.AddEdge(0, 1);
  graph.AddEdge(1, 2);
  graph.AddEdge(1, 5);
  graph.AddEdge(2, 3);
  graph.AddEdge(3, 4);
  graph.AddEdge(4, 2);

  dfs(graph, [](int v) {
    std::cout << v << std::endl;
  });

  return 0;
}
