#pragma once

#include "abstract_graph.h"

class ArcGraph : public IGraph {
  struct Edge {
    int from;
    int to;
  };

public:
  ArcGraph(size_t nvertices);
  ArcGraph(IGraph& g);

  ~ArcGraph() override = default;

  void AddEdge(int from, int to) override;

  int VerticesCount() const override;

  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

private:
  void add_edge(int from, int to);

  size_t nvertices;
  std::vector<Edge> graph;
};
