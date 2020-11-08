#include <assert.h>
#include <string.h>
#include <iostream>

#define PARENT(i) ((i-1)/2)
#define LEFT(i) (2*i + 1)
#define RIGHT(i) (2*i + 2)

#define DEFAULT_BUF_SIZE 0

bool cmp(int &a, int &b) {
  if(a <= b) {
    return true;
  }

  return false;
}

class Array {
public:
  Array() {
    capacity = DEFAULT_BUF_SIZE;
    buffer = new int[capacity];
  }

  ~Array() {
    delete [] buffer;
  }

  void push_back(int value) {
    if (size == capacity) {
      resizeBuf();
    }

    buffer[size] = value;

    size++;
  }

  int pop_back() {
    if (size == 0) {
      return -1;
    }

    int value = buffer[size - 1];
    size--;

    return value;
  }

  void push_front(int value) {
    if (size == capacity) {
      resizeBuf();
    }

    for(size_t i = size; i > 1; i--) {
      buffer[i] = buffer[i-1];
    }

    buffer[0] = value;

    size++;
  }

  int pop_front() {
    if (size == 0) {
      return -1;
    }

    int value = buffer[0];

    for(size_t i = 0; i < size-1; i++) {
      buffer[i] = buffer[i+1];
    }

    size--;
    return value;
  }

  void copy(const Array& arr) {
    if(capacity < arr.Size()) {
      resizeNBuf(arr.Capacity());
    }

    for(size_t i = 0; i < arr.Size(); i++) {
      buffer[i] = arr[i];
    }
    size = arr.Size();
  }

  void sliceCopy(const Array& arr, size_t left, size_t right) {
    assert(right > left);

    if(capacity < right - left) {
      resizeNBuf(right - left);
    }

    for(size_t i = 0; i < right - left; i++) {
      buffer[i] = arr[left + i];
    }
    size = right - left;
  }

  void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
  }

  size_t Size() const {
    return size;
  }

  size_t Capacity() const {
    return capacity;
  }

  bool isEmpty() const {
    if (size == 0) {
      return true;
    }

    return false;
  }

  int& operator[](size_t i) const {
    assert(i < size);
    return buffer[i];
  }

private:
  void resizeNBuf(size_t n) {
    capacity = n;

    delete [] buffer;

    buffer = new int[capacity];
  }

  void resizeBuf() {
    size_t prevCapacity = capacity;

   if (capacity == 0) {
      capacity = 1;
    } else {
      capacity *= 2;
    }
    int* temp = new int[capacity];

    memcpy(temp, buffer, size * sizeof(int));

    delete [] buffer;

    buffer = temp;
  }

  int* buffer = nullptr;
  size_t size = 0;
  size_t capacity = 0;
};


class Heap {
public:
  Heap(bool (*_cmp)(int&, int&)) : cmp(_cmp) {
  }

  Heap(const Array& _arr, bool (*_cmp)(int&, int&)) : cmp(_cmp) { 
    arr.copy(_arr);
    buildHeap();
  }

  Heap(const Array& _arr, size_t left, size_t right, bool (*_cmp)(int&, int&)) : cmp(_cmp) { 
    arr.sliceCopy(_arr, left, right);
    buildHeap();
  }

  ~Heap() {
  }

  void insert(int value) {
    arr.push_back(value);
    siftUp(arr.Size() - 1);
  }

  int extractMax() {
    assert(!arr.isEmpty());

    int result = arr[0];
    arr[0] = arr[arr.Size() - 1];
    arr.pop_back();

    if (!arr.isEmpty()) {
      siftDown(0);
    }

    return result;
  }

  int peekMax() const {
    assert(!arr.isEmpty());

    return arr[0];
  }

  bool isEmpty() const {
    if (arr.isEmpty()) {
      return true;
    }

    return false;
  }

private:
  void buildHeap() {
    assert(!arr.isEmpty());

    for(int i = PARENT(arr.Size() - 1); i >= 0; i--) {
      siftDown(i);
    }
  }

  void siftDown(int i) {
    int largest = i;

    while(true) {
      int left = LEFT(largest);
      int right = RIGHT(largest);

      if (left < arr.Size() && !cmp(arr[left], arr[largest])) {
        largest = left;
      }
      if (right < arr.Size() && !cmp(arr[right], arr[largest])) {
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

  void siftUp(int i) {
    while (i > 0) {
      int parent = PARENT(i);
      if (cmp(arr[i], arr[parent])) {
        return;
      }
      arr.swap(arr[i], arr[parent]);
      i = parent;
    }
  }

  Array arr = Array();
  bool (*cmp)(int&, int&) = nullptr;
};

Array find_maxs(Array& arr, int n, int windowSize) {
  Heap heap = Heap(arr, 0, windowSize, cmp);
  Heap drop = Heap(cmp);

  Array results;

  for(int i = windowSize; i < n; i++) {
    results.push_back(heap.peekMax());

    if(heap.peekMax() == arr[i - windowSize]) {
      heap.extractMax();
    } else {
      drop.insert(arr[i - windowSize]);
    }

    while(!drop.isEmpty() && drop.peekMax() == heap.peekMax()) {
      heap.extractMax();
      drop.extractMax();
    }

    heap.insert(arr[i]);
  }
  results.push_back(heap.peekMax());

  return results;
}

void run(std::istream& input, std::ostream& output) {
  int n = 0;
  input >> n;

  Array arr = Array();
  for(int i = 0; i < n; i++) {
    int value = 0;
    input >> value;

    arr.push_back(value);
  }

  int windowSize = 0;
  input >> windowSize;

  Array results = find_maxs(arr, n, windowSize);

  for(int i = 0; i < results.Size(); i++) {
    output << results[i] << " ";
  }

}

int main() {
  run(std::cin, std::cout);

  return 0;
}
