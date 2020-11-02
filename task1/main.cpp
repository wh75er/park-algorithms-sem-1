#include <iostream>
#include <string.h>
#include <assert.h>

#define DEFAULT_BUF_SIZE 8

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
  void resizeBuf() {
    size_t prevCapacity = capacity;

    capacity *= 2;
    int* temp = new int[capacity];

    memcpy(temp, buffer, size * sizeof(int));

    delete [] buffer;

    buffer = temp;
  }

  int* buffer = nullptr;
  size_t size = 0;
  size_t capacity = 0;
};

void run(std::istream& input, std::ostream& output) {
  size_t n = 0;
  input >> n;

  Array a;
  int value = 0;
  for(size_t i = 0; i < n; i++) {
    input >> value;
    a.push_back(value);
  }

  size_t m = 0;
  input >> m;

  Array b;
  value = 0;
  for(size_t i = 0; i < m; i++) {
    input >> value;
    b.push_back(value);
  }

  int k = 0;
  input >> k;


  size_t count = 0;
  int i = 0;
  int j = m-1;
  int sum = 0;
  while(i < n && j >= 0) {
    sum = a[i] + b[j];

    if (sum == k) {
      count++;
      i++;
    } else if (sum > k) {
      j--;
    } else {
      i++;
    }
  }

  output << count;
}

int main() {
  run(std::cin, std::cout);

  return 0;
}
