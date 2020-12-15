#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <chrono>

#define DEFAULT_MAP_SIZE 8

size_t str_hash(const std::string& data) {
  size_t hash = 0;
  for (char i : data) {
    hash = hash * 13 + i;
  }
  return hash;
}

template<class T> struct HashFunc;

template<> struct HashFunc<std::string> {
  size_t operator() (const std::string& key) {
    return str_hash(key);
  }
};

template<class Key, class Hash = HashFunc<Key>>
class HashMap {
  struct Bucket {
    Key key;
    bool marked = false;
  };

public:
  explicit HashMap(Hash hash = Hash()):
    items_count(0),
    max_load_factor(3.f/4.f),
    hash(hash)
  {
  };
  ~HashMap() = default;

  bool insert(const Key& key) {
    if (items_count > max_load_factor * map.size() || map.empty()) {
      grow();
    }

    size_t pos = hash(key) % map.size();

    size_t i = 1;

    bool found_deleted_pos = false;

    size_t deleted_pos = 0;

    while (pos < map.size()) {
      if (!map[pos].key.empty() && map[pos].key == key) {
        break;
      } else if (!found_deleted_pos && map[pos].key.empty() && map[pos].marked == true) {
        found_deleted_pos = true;
        deleted_pos = pos;
      } else if (map[pos].key.empty() && map[pos].marked == false) {
        if (found_deleted_pos) {
          pos = deleted_pos;
        }

        map[pos].key = key;
        map[pos].marked = true;

        items_count++;

        return true;
      }

      pos = probe(pos, i, map.size());
      i++;
    }

    return false;
  }

  ssize_t find(const Key& key) {
    if (!items_count) {
      return -1;
    }

    size_t pos = hash(key) % map.size();

    size_t i = 1;

    while(!map[pos].key.empty() || map[pos].marked) {
      if(!map[pos].key.empty() && map[pos].key == key) {
        return pos;
      }

      pos = probe(pos, i, map.size());
      i++;
    }

    return -1;
  };

  bool remove(const Key& key) {
    ssize_t pos = find(key);

    if ( pos == -1) {
      return false;
    }

    map[pos].key.clear();
    items_count--;

    if (!items_count) {
      map.erase(map.begin(), map.end());
    }

    return true;
  }

private:
  void grow() {
    if (map.empty()) {
      map.resize(DEFAULT_MAP_SIZE);
      return;
    }

    size_t new_map_size = map.size() * 2;

    std::vector<struct Bucket> new_map(new_map_size);

    for(auto const &iter : map) {
      if (!iter.key.empty()) {
        size_t pos = hash(iter.key) % new_map_size;

        size_t i = 1;

        while(pos < new_map_size) {
          if(new_map[pos].key.empty()) {
            new_map[pos].key = iter.key;
            new_map[pos].marked = true;
            break;
          }

          pos = probe(pos, i, new_map_size);
          i++;
        }
      }
    }

    map = std::move(new_map);
  }

  size_t probe(size_t idx, size_t i, size_t size) {
    return (idx + i + 1) % size;
  }


  size_t items_count = 0;
  float max_load_factor = 3.f/4.f;
  std::vector<struct Bucket> map;
  Hash hash;
};

void run(std::istream& input, std::ostream& output) {
  std::string key = "";
  char operation = '\0';

  HashMap<std::string> hmap;
  while(input >> operation >> key) {
    bool res = true;
    switch (operation) {
      case '+':
        res *= hmap.insert(key);
        break;
      case '-':
        res *= hmap.remove(key);
        break;
      case '?':
        res *= hmap.find(key) != -1;
        break;
      default:
        exit(1);
    }
    if (res) {
      output << "OK" << std::endl;
    } else {
      output << "FAIL" << std::endl;
    }
  }
}

void testHashMap() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "+ h1\n"
             "+ h2\n"
             "+ h3\n"
             "+ h4\n"
             "+ h5\n"
             "+ h6\n"
             "+ h7\n"
             "+ h8\n"
             "+ h9\n"
             "+ h10\n"
             "+ h11\n"
             "+ h12\n"
             "+ h13\n"
             "+ h14\n"
             "? h12\n"
             "- h12\n"
             "? h12\n";

    run(input, output);
    std::string ok = "OK\n";
    std::string fail = "FAIL\n";
    std::string expectation = "";

    for (auto i = 0; i < 16; i++) {
      expectation += ok;
    }
    expectation += fail;

    assert(output.str() == expectation);
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "+ h1\n"
             "+ h2\n"
             "+ h3\n"
             "+ h4\n"
             "+ h5\n"
             "+ h6\n"
             "+ h7\n"
             "+ h8\n"
             "+ h9\n"
             "+ h10\n"
             "+ h11\n"
             "+ h12\n"
             "+ h13\n"
             "+ h14\n"
             "- h14\n"
             "- h13\n"
             "- h12\n"
             "- h11\n"
             "- h10\n"
             "- h9\n"
             "- h8\n"
             "- h7\n"
             "- h6\n"
             "? h5\n";

    run(input, output);
    std::string ok = "OK\n";
    std::string fail = "FAIL\n";
    std::string expectation = "";

    for (auto i = 0; i < 24; i++) {
      expectation += ok;
    }

    assert(output.str() == expectation);
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "+ h1\n"
             "+ h2\n"
             "+ h3\n"
             "+ h4\n"
             "+ h5\n"
             "+ h6\n"
             "+ h7\n"
             "+ h8\n"
             "+ h9\n"
             "+ h10\n"
             "- h10\n"
             "- h9\n"
             "- h8\n"
             "- h7\n"
             "- h6\n"
             "- h5\n"
             "- h4\n"
             "- h3\n"
             "- h2\n"
             "- h1\n";

    run(input, output);
    std::string ok = "OK\n";
    std::string fail = "FAIL\n";
    std::string expectation = "";

    for (auto i = 0; i < 20; i++) {
      expectation += ok;
    }

    assert(output.str() == expectation);
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "+ h1\n"
             "+ h2\n"
             "+ h3\n"
             "+ h4\n"
             "+ h5\n"
             "+ h6\n"
             "+ h7\n"
             "+ h8\n"
             "+ h9\n"
             "+ h10\n"
             "- h10\n"
             "- h9\n"
             "- h8\n"
             "- h7\n"
             "- h6\n"
             "- h5\n"
             "- h4\n"
             "- h3\n"
             "- h2\n"
             "- h1\n"
             "? h1\n"
             "+ h1\n"
             "? h1\n"
             "- h1\n"
             "? h1\n";

    run(input, output);
    std::string ok = "OK\n";
    std::string fail = "FAIL\n";
    std::string expectation = "";

    for (auto i = 0; i < 20; i++) {
      expectation += ok;
    }
    expectation += fail + ok + ok + ok + fail;

    assert(output.str() == expectation);
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "+ h1\n"
             "+ h2\n"
             "+ h3\n"
             "+ h4\n"
             "+ h5\n"
             "+ h6\n"
             "+ h7\n"
             "+ h8\n"
             "+ h9\n"
             "+ h10\n"
             "- h10\n"
             "- h9\n"
             "- h8\n"
             "- h7\n"
             "- h6\n"
             "- h5\n"
             "- h4\n"
             "- h3\n"
             "- h2\n"
             "- h1\n"
             "? h1\n"
             "? h2\n"
             "? h3\n"
             "? h4\n"
             "? h5\n"
             "? h6\n"
             "? h7\n"
             "? h8\n"
             "? h9\n"
             "? h10\n";

    run(input, output);
    std::string ok = "OK\n";
    std::string fail = "FAIL\n";
    std::string expectation = "";

    for (auto i = 0; i < 20; i++) {
      expectation += ok;
    }

    for (auto i = 0; i < 10; i++) {
      expectation += fail;
    }

    assert(output.str() == expectation);
  }
}

int main() {
  auto start_time = std::chrono::high_resolution_clock::now();

  run(std::cin, std::cout);
//  testHashMap();

  auto end_time = std::chrono::high_resolution_clock::now();

  auto time = end_time - start_time;

  std::cout << "Execution time = " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << "[microsecs]" << std::endl;
}