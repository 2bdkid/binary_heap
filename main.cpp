#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <utility>
#include <sstream>

class max_heap {
public:
  max_heap(std::vector<int>::iterator begin, std::vector<int>::iterator end);
  void insert(int value);
  
private:
  friend std::ostream& operator<<(std::ostream& out, const max_heap& heap);  
  std::vector<int>::iterator parent_of(std::vector<int>::iterator child);
  std::vector<int>::iterator left_child_of(std::vector<int>::iterator parent);
  std::vector<int>::iterator right_child_of(std::vector<int>::iterator parent);
  void bubble_up(std::vector<int>::iterator elem);
  void bubble_down(std::vector<int>::iterator elem);
  
  std::vector<int> rep;
};

max_heap::max_heap(std::vector<int>::iterator begin, std::vector<int>::iterator end) {
  rep.insert(rep.begin(), begin, end);
  for (auto i = rep.end() - 1; i >= rep.begin(); --i)
    bubble_down(i);
}

std::vector<int>::iterator
max_heap::parent_of(std::vector<int>::iterator child) {
  // parent = floor((i - 1) / 2)
  const auto idx = std::distance(rep.begin(), child);
  return rep.begin() + (idx - 1) / 2;
}

std::vector<int>::iterator
max_heap::left_child_of(std::vector<int>::iterator parent) {
  // left_child = 2i - 2
  const auto idx = std::distance(rep.begin(), parent);
  return rep.begin() + (2 * idx) + 1;
}

std::vector<int>::iterator
max_heap::right_child_of(std::vector<int>::iterator parent) {
  // right_child = 2i - 1
  const auto idx = std::distance(rep.begin(), parent);
  return rep.begin() + (2 * idx) + 2;
}

void max_heap::bubble_up(std::vector<int>::iterator elem) {
  auto child = elem;
  auto parent = parent_of(child);

  // bubble up
  while (child != parent && *child > *parent) {
    std::swap(*child, *parent);
    child = parent;
    parent = parent_of(parent);
  }
}

void max_heap::bubble_down(std::vector<int>::iterator elem) {
  auto parent = elem;
  auto left_child = left_child_of(parent);
  auto right_child = right_child_of(parent);

  // stop at leaf level
  while (left_child < rep.end() || right_child < rep.end()) {
    // find maximum of parent, left_child, right_child
    auto max = parent;
    if (left_child < rep.end())
      if (*max < *left_child)
	max = left_child;
    if (right_child < rep.end())
      if (*max < *right_child)
	max = right_child;

    // heap property fixed
    if (parent == max) break;
    
    // swap with the greater child
    std::swap(*parent, *max);
    parent = max;
    left_child = left_child_of(parent);
    right_child = right_child_of(parent);
  }
}

void max_heap::insert(int value) {
  rep.push_back(value);
  bubble_up(rep.end() - 1);
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

  // linear time to build heap
  max_heap heap(arr.begin(), arr.end());
  std::cout << "heap: " << heap << '\n';
}
