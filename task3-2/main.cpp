#include <iostream>
#include <vector>
#include <queue>

struct IGraph {
  virtual ~IGraph() {}

  virtual void AddEdge(int from, int to) = 0;

  virtual int VerticesCount() const  = 0;

  virtual std::vector<int> GetNextVertices(int vertex) const = 0;
  virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

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
  void add_edge(int from, int to);

  std::vector<std::vector<int>> graph;
};

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
      add_edge(child, i);
    }
  }
}

int ListGraph::VerticesCount() const {
  return graph.size();
}

void ListGraph::AddEdge(int from, int to) {
  add_edge(from, to);
  add_edge(to, from);
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

struct Node {
  int path_depth = INT32_MAX;
  int path_count = 0;
};

template<typename Callback>
void bfs_(const IGraph& g, std::vector<bool>& visited, const int start_node, Callback callback) {
  std::queue<int> queue;

  queue.push(start_node);

  while (!queue.empty()) {
    int vertex = queue.front();
    queue.pop();

    if (!visited[vertex]) {
      visited[vertex] = true;

      std::vector<int> childs = g.GetNextVertices(vertex);

      for (auto child : childs) {
        callback(vertex, child);

        queue.push(child);
      }
    }
  }
}

template<typename Callback>
void bfs(const IGraph& g, const int start_node, Callback callback) {
  std::vector<bool> visited;

  visited.resize(g.VerticesCount());

  bfs_(g, visited, start_node, callback);

  for (size_t i = 0; i < g.VerticesCount(); i++) {
    if (!visited[i]) {
      bfs_(g, visited, i, callback);
    }
  }
}

size_t find_min_paths_count(const IGraph& graph, const int u, const int w) {
  std::vector<Node> nodes;
  nodes.resize(graph.VerticesCount());

  nodes[u].path_depth = 0;
  nodes[u].path_count = 1;

  bfs(graph, u, [&nodes](int parent, int child){
    if (nodes[parent].path_depth + 1 <= nodes[child].path_depth) {
      if (nodes[parent].path_depth + 1 == nodes[child].path_depth) {
        nodes[child].path_count += nodes[parent].path_count;
      } else {
        nodes[child].path_depth = nodes[parent].path_depth + 1;
        nodes[child].path_count = nodes[parent].path_count;
      }
    }
  });

  return nodes[w].path_count;
}

void run(std::istream& input, std::ostream& output) {
  int nvertices = 0;
  input >> nvertices;

  int nedges = 0;
  input >> nedges;

  ListGraph graph(nvertices);

  int v1 = 0;
  int v2 = 0;
  for (size_t i = 0; i < nedges; i++) {
    input >> v1 >> v2;

    graph.AddEdge(v1, v2);
  }

  input >> v1 >> v2;

  int min_paths_count = find_min_paths_count(graph, v1, v2);

  output << min_paths_count;
}

int main() {
  run(std::cin, std::cout);
}
