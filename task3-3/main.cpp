#include <iostream>
#include <vector>
#include <set>

struct IGraph {
  virtual ~IGraph() {}

  virtual void AddEdge(int v1, int v2, size_t weight) = 0;

  virtual int VerticesCount() const  = 0;

  virtual std::vector<std::pair<int, int>> GetEdges(int vertex) const = 0;
};

class ListGraph : public IGraph {

public:
  ListGraph(size_t nvertices);

  ~ListGraph() override = default;

  void AddEdge(int v1, int v2, size_t weight) override;

  int VerticesCount() const override;

  std::vector<std::pair<int, int>> GetEdges(int vertex) const override;

private:
  void add_edge(int from, int to, size_t weight);

  std::vector<std::vector<std::pair<int, int>>> graph;
};

ListGraph::ListGraph(size_t nvertices) {
  graph.resize(nvertices);
}

int ListGraph::VerticesCount() const {
  return graph.size();
}

void ListGraph::AddEdge(int v1, int v2, size_t weight) {
  graph[v1].push_back(std::make_pair(v2, weight));
  graph[v2].push_back(std::make_pair(v1, weight));
}

std::vector<std::pair<int, int>> ListGraph::GetEdges(int vertex) const {
  std::vector<std::pair<int, int>> result;

  result.resize(graph[vertex].size());
  std::copy(graph[vertex].begin(), graph[vertex].end(), result.begin());

  return result;
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

      std::vector<std::pair<int, int>> edges = g.GetEdges(vertex);

      for (auto edge : edges) {
        if (vertices_weights[vertex] + edge.second < vertices_weights[edge.first]) {
          if (vertices_weights[edge.first] != INT32_MAX) {
            set.erase(std::make_pair(edge.first, vertices_weights[edge.first]));
          }

          vertices_weights[edge.first] = vertices_weights[vertex] + edge.second;
          visited[edge.first] = false;
        }

        set.emplace(std::make_pair(edge.first, vertices_weights[edge.first]));
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

  ListGraph graph(nvertices);

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
