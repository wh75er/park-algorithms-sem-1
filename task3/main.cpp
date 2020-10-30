#include <assert.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <chrono>

#define DEFAULT_BUF_SIZE 8

class RingQueue {
  enum Direction {
    Left,
    Right,
  };

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
        ringSize++;

        Direction dir = shiftValues(popi);

        if(dir == Left) {
          pushi = decrement(pushi);
          popi = decrement(popi);
        }
      } else if(!isFull() && !isEmpty()) {
        popi = decrement(popi);
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

      if(isFull() || isEmpty()) {
        ringSize++;
        Direction dir = shiftValues(popi);

        if(dir == Left) {
          pushi = decrement(pushi);
        } else {
          popi = (popi + 1) % ringSize;
        }
      }

      buffer[pushi] = value;
      pushi = (pushi + 1) % ringSize;

      volume++;
    }

    int pop_back() {
      if(isEmpty()) {
        return -1;
      }

      pushi = decrement(pushi);
      
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

    size_t decrement(size_t value) {
      if(value == 0) {
        value = ringSize - 1;
      } else {
        value--;
      }

      return value;
    } 

    void shiftRight(size_t value) {
      for(size_t i = ringSize - 1; i != value; i = decrement(i)) {
        buffer[i] = buffer[decrement(i)];
      }
    }

    void shiftLeft(size_t value) {
      for(size_t i = ringSize - 1; i != value; i = (i+1) % ringSize) {
        buffer[i % ringSize] = buffer[(i + 1) % ringSize];
      }
    }

    Direction shiftValues(size_t value) {
      size_t mid = ringSize / 2;

      if(value < mid) {
        shiftLeft(value);
        return Left;
      }

      shiftRight(value);
      return Right;
    }

    void resizeBuf() {
      bufSize *= 2;
      int* temp = new int[bufSize];
      
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
  {
    std::stringstream input;
    std::stringstream output;
    input << "7 3 11 3 22 3 33 2 11 2 22 2 33 2 -1";
    run(input, output);
    assert(output.str() == "YES");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "9 3 11 3 22 3 33 1 00 2 00 2 11 2 22 2 33 2 -1";
    run(input, output);
    assert(output.str() == "YES");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "27 3 11 3 22 3 33 1 00 2 00 2 11 1 55 3 66 3 77 3 88 3 99 3 99 3 99 3 99 3 99 4 99 4 99 4 99 4 99 4 99 2 55 2 22 2 33 2 66 4 88 2 77 2 -1"; 
    run(input, output);
    assert(output.str() == "YES");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "3 1 11 2 11 2 -1";
    run(input, output);
    assert(output.str() == "YES");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "3 3 11 2 11 2 -1";
    run(input, output);
    assert(output.str() == "YES");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "19 3 11 3 22 3 33 3 44 3 55 2 11 2 22 2 33 3 66 3 77 3 88 3 99 4 99 4 88 4 77 4 66 2 44 2 55 4 -1";
    run(input, output);
    assert(output.str() == "YES");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "17 3 11 3 22 3 33 3 44 3 55 2 11 2 22 2 33 1 66 1 77 3 88 4 88 4 55 4 44 4 66 4 77 4 -1";
    run(input, output);
    assert(output.str() == "YES");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "21 3 11 3 22 3 33 3 44 3 55 2 11 2 22 2 33 3 66 1 77 1 88 1 99 1 10 4 66 4 55 4 44 4 77 4 88 4 99 4 10 4 -1";
    run(input, output);
    assert(output.str() == "YES");
  }
}

int main() {
//  auto start = std::chrono::system_clock::now();
  run(std::cin, std::cout);
//  auto end = std::chrono::system_clock::now();
//  std::chrono::duration<double> elapsed = end-start;
//  auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
//  std::cout << "Time: " << millis  << "ms" << std::endl;

//  testQueue();

  return 0;
}
