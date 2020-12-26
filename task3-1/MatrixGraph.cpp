#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(size_t nvertices) {
  graph.resize(nvertices, std::vector<bool>(nvertices));
}

MatrixGraph::MatrixGraph(IGraph &g) {
  size_t nvertices = g.VerticesCount();
  graph.resize(nvertices, std::vector<bool>(nvertices));

  for (size_t i = 0; i < nvertices; i++) {
    std::vector<int> childs = g.GetNextVertices(i);
    for (auto& child : childs) {
      add_edge(i, child);
    }
  }
}

int MatrixGraph::VerticesCount() const {
  return graph.size();
}

void MatrixGraph::AddEdge(int from, int to) {
  add_edge(from, to);
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
  std::vector<int> result;

  for (size_t i = 0; i < graph.size(); i++) {
    if (graph[vertex][i]) {
      result.push_back(i);
    }
  }

  return result;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
  std::vector<int> result;

  for (size_t i = 0; i < graph.size(); i++) {
    if (i != vertex && graph[i][vertex]) {
      result.push_back(i);
    }
  }

  return result;
}

void MatrixGraph::add_edge(int from, int to) {
  graph[from][to] = true;
}