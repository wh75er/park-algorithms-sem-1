#include <iostream>

#define DEFAULT_BUF_SIZE 8

class Array {
public:
  Array();
  ~Array();
}

class Heap {
public:
  Heap();
  explicit Heap(const int& _arr);
  ~Heap();

  void insert(int value);

  int extractMax();

  int peekMax() const;

private:
  int* buffer;

  void buildHeap();
  void siftDown(int i);
  void siftUp(int i)
};
