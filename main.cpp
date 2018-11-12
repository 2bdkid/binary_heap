#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <utility>
#include <sstream>

class max_heap {
public:
  void insert(int value);
  
private:
  friend std::ostream& operator<<(std::ostream& out, const max_heap& heap);  
  std::vector<int>::iterator parent_of(std::vector<int>::iterator child);
  std::vector<int> rep;
};

std::vector<int>::iterator
max_heap::parent_of(std::vector<int>::iterator child) {
  // parent = floor((i - 1) / 2)
  const auto idx = std::distance(rep.begin(), child);
  return rep.begin() + (idx - 1) / 2;
}

void max_heap::insert(int value) {
  rep.push_back(value);
  
  auto child = rep.end() - 1;
  auto parent = parent_of(child);

  // bubble up
  while (child != parent && *child > *parent) {
    std::swap(*child, *parent);
    child = parent;
    parent = parent_of(parent);
  }
}

std::ostream& operator<<(std::ostream& out, const max_heap& heap) {
  // output contents of heap.rep
  if (heap.rep.size() > 0) {
    out << heap.rep.front();
    for (auto i = 1u; i < heap.rep.size(); ++i)
      out << ' ' << heap.rep[i];
  }
  return out;
}

int main() {
  std::string line;

  // get set of integers from stdin
  do {
    std::cout << "insert set of integers: ";
    std::getline(std::cin, line);
  } while (line == "");

  // parse stdin input into vector<int>
  std::stringstream stream(line);
  std::vector<int> arr;
  
  while (std::getline(stream, line, ' ')) {
    try {
      int n = stoi(line);
      arr.push_back(n);
    } catch (...) {
      continue;
    }
  }

  // fill heap using naive method
  max_heap heap;
  
  for (const auto n : arr)
    heap.insert(n);

  std::cout << "heap: " << heap << '\n';
}
