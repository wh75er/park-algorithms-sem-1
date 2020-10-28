#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sstream>

#define DEFAULT_BUF_SIZE 8

class RingQueue {
  public:
    RingQueue() {
      buffer = new int[bufSize];
    }

    ~RingQueue() {
      delete [] buffer;
    }

    void push_front(int value) {
      if(isOverflow()) {
        resizeBuf();
      }

      if(isFull() || isEmpty()) {
        shiftElements(popi, ringSize, ringSize+1);
        ringSize++;
      } else if(!isFull() && !isEmpty()) {
        shiftElements(popi, pushi, ringSize);
        pushi = (pushi + 1) % ringSize;
      }

      buffer[popi] = value;

      volume++;
    }

    int pop_front() {
      if(isEmpty()) {
        return -1;
      }

      int value = buffer[popi];

      popi = (popi + 1) % ringSize;

      volume--;

      return value;
    }

    void push_back(int value) {
      if(isOverflow()) {
        resizeBuf();
      }

      if(isEmpty()) {
        ringSize++;
      } else if(isFull()) {
        if(pushi == popi && popi == 0) {
          pushi = ringSize;
        } else {
          shiftElements(popi, ringSize, ringSize+1);
          popi++;
        }
        ringSize++;
      }

      buffer[pushi] = value;
      pushi = (pushi + 1) % ringSize;

      volume++;
    }

    int pop_back() {
      if(isEmpty()) {
        return -1;
      }

      if(pushi == 0) {
        pushi = ringSize-1;
      } else {
        pushi--;
      }
      
      volume--;

      return buffer[pushi];
    }

  private:
    bool isOverflow() {
      if(ringSize == bufSize) {
        return true;
      }

      return false;
    }

    bool isEmpty() {
      if(!volume){
        return true;
      }

      return false;
    }

    bool isFull() {
      if(volume && pushi == popi) {
        return true;
      }

      return false;
    }

    void shiftElements(int left, int right, size_t size) {
      for(size_t i = left; i % size < right; i++) {
        buffer[(i+1) % size] = buffer[i % size];
      }
    }

    void resizeBuf() {
      bufSize *= 2;
      int* temp = new int(bufSize);
      
      memcpy(temp, buffer, ringSize * sizeof(int));
      delete [] buffer;
      buffer = temp;
    }

    size_t popi = 0;
    size_t pushi = 0;
    size_t volume = 0;
    size_t ringSize = 0;
    size_t bufSize = DEFAULT_BUF_SIZE;
    int* buffer = nullptr;
};

void run(std::istream& input, std::ostream& output) {
  RingQueue q;

  int n = 0;
  input >> n;

  bool result = true;
  for(int i = 0; i < n; i++){
    int command = 0;
    int value = 0;

    input >> command >> value;
    switch(command) {
      case 1:
        q.push_front(value);
        break;
      case 2:
        result = result && q.pop_front() == value;
        break;
      case 3:
        q.push_back(value);
        break;
      case 4:
        result = result && q.pop_back() == value;
        break;
    }
  }

  output << (result ? "YES" : "NO");
}

void testQueue() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "3 3 44 3 50 2 44";
    run(input, output);
    assert(output.str() == "YES");
  }
}

int main() {
  run(std::cin, std::cout);

//  testQueue();

  return 0;
}
