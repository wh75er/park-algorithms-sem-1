#include "ArcGraph.h"

ArcGraph::ArcGraph(size_t nvertices_) {
  nvertices = nvertices_;
}

ArcGraph::ArcGraph(IGraph &g) {
  nvertices = g.VerticesCount();

  for (size_t i = 0; i < nvertices; i++) {
    std::vector<int> childs = g.GetNextVertices(i);
    for (auto& child : childs) {
      add_edge(i, child);
    }
  }
}

int ArcGraph::VerticesCount() const {
  return nvertices;
}

void ArcGraph::AddEdge(int from, int to) {
  add_edge(from, to);
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
  std::vector<int> result;

  for (auto& edge : graph) {
    if (edge.from == vertex) {
      result.push_back(edge.to);
    }
  }

  return result;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
  std::vector<int> result;

  for (auto& edge : graph) {
    if (edge.to == vertex) {
      result.push_back(edge.from);
    }
  }

  return result;
}

void ArcGraph::add_edge(int from, int to) {
  graph.push_back(Edge {from, to});
}