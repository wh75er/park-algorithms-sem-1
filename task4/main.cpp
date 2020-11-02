#include <iostream>

#define PARENT(i) (i-1/2)
#define LEFT(i) (2*i + 1)
#define RIGHT(i) (2*i + 2)

#define DEFAULT_BUF_SIZE 8

template <typename T>
class Array {
public:
  Array() {
    capacity = DEFAULT_BUF_SIZE;
    buffer = new T[capacity];
  }

  ~Array() {
    delete buffer;
  }

  T pop_back();

  void push_back(T value);
  
  T pop_front();

  void push_front(T value);

  void swap(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
  }

private:
  resizeBuf() {
  }

  T* buffer = nullptr;
  size_t size = 0;
  size_t capacity = 0;
}

class Heap {
public:
  Heap();
  explicit Heap(const int& _arr);
  ~Heap();

  void insert(int value);

  int extractMax();

  int peekMax() const {
    if (!arr) {
      return;
    }

    return arr[0];
  }

private:
  Array* arr = nullptr;

  void buildHeap() {
    if (!arr) {
      return;
    }

    for(size_t i = PARENT(arr.size() - 1); i > 0; i--) {
      siftDown(i);
    }
  }

  void siftDown(size_t i) {
    if (!arr) {
      return;
    }

    int largest = i;

    while(true) {
      size_t left = LEFT(largest);
      size_t right = RIGHT(largest);

      if (left < arr.size() && arr[left] > largest) {
        largest = left;
      }
      if (right < arr.size() && arr[right] > largest) {
        largest = right;
      }
      if (largest != i) {
        arr.swap(arr[i], arr[largest]);
        i = largest;
      } else {
        break;
      }
    }
  }

  void siftUp(size_t i) {
    if (!arr) {
      return;
    }

    while (i > 0) {
      size_t parent = PARENT(i);
      if (arr[i] <= arr[parent]) {
        return;
      }
      arr.swap(arr[i], arr[parent]);
      i = parent;
    }
  }
};
