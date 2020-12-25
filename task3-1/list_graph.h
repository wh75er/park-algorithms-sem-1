#pragma once

#include "abstract_graph.h"

class ListGraph : public IGraph {
public:
  ListGraph(size_t nvertices);
  ListGraph(IGraph& graph);

  ~ListGraph() override = default;

  void AddEdge(int from, int to) override;

  int VerticesCount() const override;

  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

private:
  std::vector<std::vector<int>> graph;
};