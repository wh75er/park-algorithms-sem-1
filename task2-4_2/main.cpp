/*
 *
 * Порядковые статистики.
 * Дано число N и N строк.
 * Каждая строка содержит команду добавления или удаления натуральных чисел,
 * а также запрос на получение k-ой порядковой статистики.
 * Команда добавления числа A задается положительным числом A,
 * команда удаления числа A задается отрицательным числом “-A”.
 * Запрос на получение k-ой порядковой статистики задается числом k.
 * Требования: скорость выполнения запроса - O(log n).
 *
 */

#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <stack>
#include <vector>


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

template<class Key, class Comparator=DefaultComparator<Key>>
class AVLTree {
  struct Node {
    Node* left;
    Node* right;

    Key key;

    uint8_t height;

    Node(const Key& key):
      left(nullptr), right(nullptr),
      key(key), height(1) {
    }
  };

public:
  AVLTree():
    root(nullptr), nodes_count(0) {
  }

  ~AVLTree() { delete root; }

  bool find(const Key& key) {
    return find_(key, root);
  };

  void insert(const Key& key) {
    root = insert_(key, root);
  }

  void erase(const Key& key) {
    root = erase_(key, root);
  }

  size_t size() {
    return nodes_count;
  }

  template<typename Lambda>
  void traverse(size_t n, Lambda&& callback) {
    if (n > nodes_count || !root) {
      return;
    }

    std::stack<Node*> stack;

    Node* curr = root;

    while(n) {
      if (curr) {
        stack.push(curr);
        curr = curr->left;
      } else {
        if (stack.empty()) {
          return;
        }

        curr = stack.top();
        stack.pop();

        callback(curr);

        curr = curr->right;

        n--;
      }
    }
  }

private:
  Node* find_(const Key& key, Node* node) {
    if (!node) {
      return nullptr;
    }

    int res = comp(key, node->key);
    if (res == -1) {
      return find_(key, node->left);
    } else if (res == 1){
      return find_(key, node->right);
    } else {
      return node;
    }
  }

  Node* insert_(const Key& key, Node* node) {
    if (!node) {
      nodes_count++;
      return new Node(key);
    }

    int res = comp(key, node->key);
    if (res == -1) {
      node->left = insert_(key, node->left);
    } else {
      node->right = insert_(key, node->right);
    }

    return balance(node);
  }

  Node* erase_(const Key& key, Node* node) {
    if (!node) {
      return node;
    }

    int res  = comp(key, node->key);
    if (res == -1) {
      node->left = erase_(key, node->left);
    } else if (res == 1) {
      node->right = erase_(key, node->right);
    } else {
      nodes_count--;

      Node* left = node->left;
      Node* right = node->right;

      delete node;

      if (!right) {
        return left;
      }

      Node* min_node = raise_min(right);
      min_node->left = left;

      return balance(min_node);
    }

    return balance(node);
  }

  Node* raise_min(Node* node) {
    Node* start = node;

    Node* min_node = node;

    Node* pre_min = nullptr;

    while(min_node->left) {
      pre_min = min_node;
      min_node = min_node->left;
    }

    if(!pre_min) {
      return start;
    }

    pre_min->left = min_node->right;
    min_node->right = start;

    return balance(min_node);
  }

  uint8_t height(Node* node) {
    if (!node) {
      return 0;
    }

    return node->height;
  }

  void fix_height(Node* node) {
    if (!node) {
      return;
    }

    node->height = std::max(height(node->left), height(node->right)) + 1;
  }

  uint8_t bfactor(Node* node) {
    return height(node->right) - height(node->left);
  }

  Node* balance(Node* node) {
    fix_height(node);

    int bf = bfactor(node);
    if (bf == 2) {
      if (bfactor(node->right) < 0) {
        node->right = rotate_right(node);
      }

      return rotate_left(node);
    } else if (bf == -2) {
      if(bfactor(node->left) > 0) {
        node->left = rotate_left(node->left);
      }

      return rotate_right(node);
    }

    return node;
  }

  Node* rotate_left(Node* upper) {
    Node* child = upper->right;

    upper->right = child->left;

    child->left = upper;

    fix_height(upper);
    fix_height(child);

    return child;
  }

  Node* rotate_right(Node* upper) {
    Node* child = upper->left;

    upper->left = child->right;

    child->right = upper;

    fix_height(upper);
    fix_height(child);

    return child;
  }

  Node* root = nullptr;
  size_t nodes_count = 0;

  Comparator comp = Comparator();
};

void run(std::istream& input, std::ostream& output) {
  AVLTree<int> tree;

  int n = 0;
  int value = 0;
  int k = 0;
  input >> n;

  while(n)  {
    input >> value >> k;

    if (value > 0) {
      tree.insert(value);
    } else if (value < 0) {
      tree.erase(std::abs(value));
    }

    std::vector<int> result;
    tree.traverse(k+1, [&result]<typename Node>(Node* node){result.push_back(node->key);});

    output << result.back() << std::endl;

    n--;
  }
}


int main() {
//  AVLTree<int> tree;
//  for (int i = 0; i < 100; i++) {
//    tree.insert(i);
//    assert(tree.find(i) == true);
//  }
//  for (int i = 0; i < 100; i++) {
//    assert(tree.find(i) == true);
//    tree.erase(i);
//    assert(tree.find(i) == false);
//  }
//  assert(!tree.size());
//  std::cout << "Ok" << std::endl;

  run(std::cin, std::cout);

  return 0;
}