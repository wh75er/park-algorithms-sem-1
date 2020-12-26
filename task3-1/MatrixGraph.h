#pragma once

#include "abstract_graph.h"

class MatrixGraph : public IGraph {
public:
  MatrixGraph(size_t nvertices);
  MatrixGraph(IGraph& g);

  ~MatrixGraph() override = default;

  void AddEdge(int from, int to) override;

  int VerticesCount() const override;

  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

private:
  void add_edge(int from, int to);

  std::vector<std::vector<bool>> graph;
};
