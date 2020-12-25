#include "list_graph.h"

ListGraph::ListGraph(size_t nvertices) {
  graph.resize(nvertices);
}

int ListGraph::VerticesCount() const {
  return graph.size();
}

void ListGraph::AddEdge(int from, int to) {
  graph[from].push_back(to);
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
  std::vector<int> result;

  result.resize(graph[vertex].size());
  std::copy(graph[vertex].begin(), graph[vertex].end(), result.begin());

  return result;
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
  std::vector<int> result;

  for (size_t i = 0; i < graph.size(); i++) {
    if (i != vertex) {
      for (auto& children : graph[i]) {
        if (children == vertex) {
          result.push_back(i);
          break;
        }
      }
    }
  }

  return result;
}