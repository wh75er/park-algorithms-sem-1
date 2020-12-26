#pragma once

#include <unordered_set>

#include "abstract_graph.h"

class SetGraph : public IGraph {
public:
  SetGraph(size_t nvertices);
  SetGraph(IGraph& g);

  ~SetGraph() override = default;

  void AddEdge(int from, int to) override;

  int VerticesCount() const override;

  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

private:
  void add_edge(int from, int to);

  std::vector<std::unordered_set<int>> graph;
};
