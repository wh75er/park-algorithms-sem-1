#include <iostream>
#include <vector>
#include <set>

struct Edge {
  int v1 = 0;
  int v2 = 0;
  size_t weight = 0;
};

struct IGraph {
  virtual ~IGraph() {}

  virtual void AddEdge(int v1, int v2, size_t weight) = 0;

  virtual int VerticesCount() const  = 0;

  virtual std::vector<Edge> GetEdges(int vertex) const = 0;
};

class ArcGraph : public IGraph {

public:
  ArcGraph(size_t nvertices);

  ~ArcGraph() override = default;

  void AddEdge(int v1, int v2, size_t weight) override;

  int VerticesCount() const override;

  std::vector<Edge> GetEdges(int vertex) const override;

private:
  void add_edge(int from, int to, size_t weight);

  size_t nvertices;
  std::vector<Edge> graph;
};

ArcGraph::ArcGraph(size_t nvertices_) {
  nvertices = nvertices_;
}

int ArcGraph::VerticesCount() const {
  return nvertices;
}

void ArcGraph::AddEdge(int v1, int v2, size_t weight) {
  graph.push_back(Edge {v1, v2, weight});
}

std::vector<Edge> ArcGraph::GetEdges(int vertex) const {
  std::vector<Edge> result;

  for (auto& edge : graph) {
    if (edge.v1 == vertex || edge.v2 == vertex) {
      result.push_back(edge);
    }
  }

  return result;
}

void ArcGraph::add_edge(int from, int to, size_t weight) {
}


void dijkstra_(const IGraph& g, std::vector<bool>& visited, std::vector<size_t>& vertices_weights, const int start_node) {
  std::set<std::pair<int, int>> set;

  set.emplace(std::make_pair(start_node, vertices_weights[start_node]));

  while (!set.empty()) {
    auto top = set.begin();
    int vertex = top->first;
    set.erase(top);

    if (!visited[vertex]) {
      visited[vertex] = true;

      std::vector<Edge> edges = g.GetEdges(vertex);

      for (auto edge : edges) {
        int child = edge.v1 == vertex ? edge.v2 : edge.v1;

        if (vertices_weights[vertex] + edge.weight < vertices_weights[child]) {
          if (vertices_weights[child] != INT32_MAX) {
            set.erase(std::make_pair(child, vertices_weights[child]));
          }

          vertices_weights[child] = vertices_weights[vertex] + edge.weight;
          visited[child] = false;
        }

        set.emplace(std::make_pair(child, vertices_weights[child]));
      }
    }
  }
}

std::vector<size_t> dijkstra(const IGraph& g, const int start_node) {
  std::vector<bool> visited;
  std::vector<size_t> vertices_weights(g.VerticesCount(), INT32_MAX);
  vertices_weights[start_node] = 0;

  visited.resize(g.VerticesCount());

  dijkstra_(g, visited, vertices_weights, start_node);

  for (size_t i = 0; i < g.VerticesCount(); i++) {
    if (!visited[i]) {
      dijkstra_(g, visited, vertices_weights, i);
    }
  }

  return vertices_weights;
}

size_t find_min_path_length(const IGraph& g, int u, int w) {

  std::vector<size_t> vertices_weights = dijkstra(g, u);

  return vertices_weights[w];
}

void run(std::istream& input, std::ostream& output) {
  int nvertices = 0;
  input >> nvertices;

  int nedges = 0;
  input >> nedges;

  ArcGraph graph(nvertices);

  int v1 = 0;
  int v2 = 0;
  int weight = 0;
  for (size_t i = 0; i < nedges; i++) {
    input >> v1 >> v2 >> weight;

    graph.AddEdge(v1, v2, weight);
  }

  input >> v1 >> v2;

  int min_path_len = find_min_path_length(graph, v1, v2);

  output << min_path_len;
}

int main() {
  run(std::cin, std::cout);
}
