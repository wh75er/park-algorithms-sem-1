#include "ListGraph.h"

ListGraph::ListGraph(size_t nvertices) {
  graph.resize(nvertices);
}

ListGraph::ListGraph(IGraph &g) {
  size_t nvertices = g.VerticesCount();
  graph.resize(nvertices);

  for (size_t i = 0; i < nvertices; i++) {
    std::vector<int> childs = g.GetNextVertices(i);
    for (auto& child : childs) {
      add_edge(i, child);
    }
  }
}

int ListGraph::VerticesCount() const {
  return graph.size();
}

void ListGraph::AddEdge(int from, int to) {
  add_edge(from, to);
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

void ListGraph::add_edge(int from, int to) {
  graph[from].push_back(to);
}