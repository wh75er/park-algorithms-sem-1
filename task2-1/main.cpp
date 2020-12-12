#include <iostream>
#include <vector>
#include <string>

#define DEFAULT_MAP_SIZE 8

size_t str_hash(const std::string& data) {
  size_t hash = 0;
  for (char i : data) {
    hash = hash * 31 + i;
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
  HashMap(Hash hash = Hash()):
    items_count(0),
    max_load_factor(3/4),
    hash(hash)
  {
  };
  ~HashMap() = default;

  bool insert(const Key& key) {
    if (items_count > max_load_factor * map.size() || !map.size()) {
      grow();
    }

    if (find(key)) {
      return false;
    }

    size_t pos = hash(key) % map.size();

    auto iter = map.begin() + pos;

    size_t i = 1;

    while(iter < map.end()) {
      if(iter->key.empty()) {
        iter->key = key;
        iter->marked = true;

        items_count++;

        return true;
      }

      iter += i * i;
      i++;
    }

    return false;
  }

  bool find(const Key& key) {
    if (map.empty()) {
      return false;
    }

    size_t pos = hash(key) % map.size();

    auto iter = map.begin() + pos;

    size_t i = 1;

    while(iter < map.end() && (!iter->key.empty() || iter->marked)) {
      if(!iter->key.empty() && iter->key == key) {
        return true;
      }

      iter += i * i;
      i++;
    }

    return false;
  };

  bool remove(const Key& key) {
    if (!find(key)) {
      return false;
    }

    size_t pos = hash(key) % map.size();

    auto iter = map.begin() + pos;

    size_t i = 1;

    while(iter < map.end()) {
      if(iter->key == key) {
        iter->key.clear();
        break;
      }

      iter += i * i;
      i++;
    }

    items_count--;

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

    for(auto iter : map) {
      if (!iter.key.empty()) {
        size_t pos = hash(iter.key) % new_map_size;

        auto new_iter = new_map.begin() + pos;

        size_t i = 1;

        while(new_iter < new_map.end()) {
          if(new_iter->key.empty()) {
            new_iter->key = iter.key;
            new_iter->marked = true;
            break;
          }

          new_iter += i * i;
          i++;
        }
      }
    }

    map = new_map;
  }

  size_t items_count = 0;
  size_t max_load_factor = 3/4;
  std::vector<struct Bucket> map;
  Hash hash;
};

int main() {
  std::string key = "";
  char operation = '\0';

  HashMap<std::string> hmap;
  while(std::cin >> operation >> key) {
    bool res = false;
    switch (operation) {
      case '+':
        res = hmap.insert(key);
        break;
      case '-':
        res = hmap.remove(key);
        break;
      case '?':
        res = hmap.find(key);
        break;
      default:
        return 1;
    }
    if (res) {
      std::cout << "OK" << std::endl;
    } else {
      std::cout << "FAIL" << std::endl;
    }
  }
}
