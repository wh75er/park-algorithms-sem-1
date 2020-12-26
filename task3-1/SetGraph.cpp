#include "SetGraph.h"

#include <iostream>

SetGraph::SetGraph(size_t nvertices) {
  graph.resize(nvertices);
}

int SetGraph::VerticesCount() const {
  return graph.size();
}

SetGraph::SetGraph(IGraph &g) {
  size_t nvertices = g.VerticesCount();
  graph.resize(nvertices);

  for (size_t i = 0; i < nvertices; i++) {
    std::vector<int> childs = g.GetNextVertices(i);
    for (auto& child : childs) {
      add_edge(i, child);
    }
  }
}

void SetGraph::AddEdge(int from, int to) {
  add_edge(from, to);
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
  std::vector<int> result;

  for (auto& child : graph[vertex]) {
    result.push_back(child);
  }

  return result;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
  std::vector<int> result;

  for (size_t i = 0; i < graph.size(); i++) {
    if (i != vertex) {
      if (graph[i].find(vertex) != graph[i].end()) {
        result.push_back(i);
      }
    }
  }

  return result;
}

void SetGraph::add_edge(int from, int to) {
  graph[from].insert(to);
}

