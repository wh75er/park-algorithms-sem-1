#include <iostream>
#include <vector>
#include <string>

#define TABLE_INIT_SIZE 8

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
public:
  HashMap(Hash hash = Hash()):
    items_count(0),
    max_load_factor(3/4),
    hash(hash)
  {
  };
  ~HashMap() = default;

  bool insert(const Key& key);
  bool find(const Key& key);
  bool remove(const Key& key);

private:
  void grow();

  size_t items_count = 0;
  size_t max_load_factor = 3/4;
  std::vector<Key> map;
  Hash hash;
};

int main() {
  return 0;
}
