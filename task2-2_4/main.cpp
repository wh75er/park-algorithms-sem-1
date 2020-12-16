#include <queue>
#include <istream>
#include <iostream>

template <class T>
struct DefaultComparator {
  int operator() (const T& l, const T& r) const {
    if (l < r) {
      return -1;
    }
    if (l == r) {
      return 0;
    }
    return 1;
  }
};

template <class Value, class Comparator=DefaultComparator<Value>>
class BinaryTree {
  struct Node {
    Node* left;
    Node* right;

    Value value;

    Node(const Value& v) :
      left(nullptr), right(nullptr),
      value(v) {
    }
  };
public:
  BinaryTree(Comparator comp = Comparator()) :
    root(nullptr), comp(comp) {
  }

  ~BinaryTree() {
    std::vector<Node*> nodes;

    traverse([&nodes](Node* node){nodes.push_back(node);});

    for (auto& node : nodes) {
      delete node;
    }
  }

  void insert(Value v) {
    Node* curr = root;

    if (!curr) {
      root = new Node(v);
      return;
    }

    while (curr) {
      if (comp(v, curr->value) == -1) {
        if (!curr->left) {
          Node* new_node = new Node(v);

          curr->left = new_node;

          curr = nullptr;
        } else {
          curr = curr->left;
        }
      } else {
        if (!curr->right) {
          Node* new_node = new Node(v);

          curr->right = new_node;

          curr = nullptr;
        } else {
          curr = curr->right;
        }
      }
    }
  }

  template<typename Lambda>
  void traverse(Lambda&& callback) {
    std::queue<Node*> queue;
    if (root) {
      queue.push(root);
    }

    while (!queue.empty()) {
      Node* curr = queue.front();
      queue.pop();

      callback(curr);

      if (curr->left) {
        queue.push(curr->left);
      }

      if (curr->right) {
        queue.push(curr->right);
      }
    }
  }

private:
  Node* root = nullptr;
  Comparator comp = Comparator();
};

void run(std::istream& input, std::ostream& output) {
  BinaryTree<int> tree;

  int n = 0;
  int value = 0;
  input >> n;

  while(n)  {
    input >> value;

    tree.insert(value);

    n--;
  }

  tree.traverse([&output]<typename Node>(Node* node){output << node->value << " ";});
}


int main() {
  run(std::cin, std::cout);
}