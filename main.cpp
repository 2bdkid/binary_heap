#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <utility>
#include <sstream>
#include <cmath>

template<typename Type>
class max_heap {
public:
  using value_type = Type;
  using reference = Type&;
  using const_reference = const Type&;
  using iterator = typename std::vector<Type>::iterator;
  using const_iterator = typename std::vector<Type>::const_iterator;
  using difference_type = typename std::vector<Type>::difference_type;
  using size_type = typename std::vector<Type>::size_type;

  max_heap() = default;
  ~max_heap() = default;
  max_heap(iterator begin, iterator end);
  max_heap(const max_heap<Type>& other);
  max_heap(max_heap<Type>&& other);

  iterator begin();
  const_iterator begin() const;
  const_iterator cbegin() const;
  iterator end();
  const_iterator end() const;
  const_iterator cend() const;

  reference operator=(max_heap<Type> other);
  reference operator=(max_heap<Type>&& other);
  bool operator==(const max_heap<Type>& other);
  bool operator!=(const max_heap<Type>& other);
  void swap(max_heap& other);
  template<typename T>
  friend void swap(max_heap<T>& lhs, max_heap<T>& rhs);
  size_type size() const;
  size_type max_size() const;
  bool empty() const;
  
  void sort();
  void insert(value_type value);
  void remove(iterator elem);
  void remove_maximum();
  reference maximum() const;

  // build tree in linear time
  void build();
  
private:
  iterator parent_of(iterator child);
  iterator left_child_of(iterator parent);
  iterator right_child_of(iterator parent);
  void bubble_up(iterator elem);
  void bubble_down(iterator elem);
  void bubble_down(iterator elem, iterator last);
  
  std::vector<int> rep;
};

template<typename Type>
max_heap<Type>::max_heap(iterator begin, iterator end)
  : rep(begin, end) {
  build();
}

template<typename Type>
max_heap<Type>::max_heap(const max_heap<Type>& other)
  : rep(other.rep) { }

template<typename Type>
max_heap<Type>::max_heap(max_heap<Type>&& other) {
  std::swap(rep, other.rep);
}

template<typename Type>
typename max_heap<Type>::iterator
max_heap<Type>::begin() { return rep.begin(); }

template<typename Type>
typename max_heap<Type>::const_iterator
max_heap<Type>::begin() const { return rep.begin(); }

template<typename Type>
typename max_heap<Type>::const_iterator
max_heap<Type>::cbegin() const { return rep.begin(); }

template<typename Type>
typename max_heap<Type>::iterator
max_heap<Type>::end() { return rep.end(); }

template<typename Type>
typename max_heap<Type>::const_iterator
max_heap<Type>::end() const { return rep.end(); }

template<typename Type>
typename max_heap<Type>::const_iterator
max_heap<Type>::cend() const { return rep.end(); }

template<typename Type>
typename max_heap<Type>::reference
max_heap<Type>::operator=(max_heap<Type> other) {
  // copy-swap
  std::swap(rep, other.rep);
  return *this;
}

template<typename Type>
typename max_heap<Type>::reference
max_heap<Type>::operator=(max_heap<Type>&& other) {
  // copy-swap
  std::swap(rep, other.rep);
  return *this;
}

template<typename Type>
bool max_heap<Type>::operator==(const max_heap<Type>& other) {
  return std::equal(begin(), end(), other.begin(), other.end());
}

template<typename Type>
bool max_heap<Type>::operator!=(const max_heap<Type>& other) {
  return !operator==(other);
}

template<typename Type>
void max_heap<Type>::swap(max_heap<Type>& other) {
  std::swap(rep, other.rep);
}

template<typename Type>
void swap(max_heap<Type>& lhs, max_heap<Type> rhs) {
  lhs.swap(rhs);
}

template<typename Type>
typename max_heap<Type>::size_type
max_heap<Type>::size() const { return rep.size(); }

template<typename Type>
typename max_heap<Type>::size_type
max_heap<Type>::max_size() const { return rep.max_size(); }

template<typename Type>
bool max_heap<Type>::empty() const { return rep.empty(); }

template<typename Type>
void max_heap<Type>::build() {
  // skip leaf nodes  
  const auto n = begin() + std::ceil(rep.size() / 2);
  for (auto i = n; i >= begin(); --i)
    bubble_down(i);
}

template<typename Type>
void max_heap<Type>::sort() {
  auto iter = end() - 1;

  while (iter >= begin()) {
    std::swap(*begin(), *iter);
    // bubble root down but ignore elements past iter
    bubble_down(begin(), iter);
    --iter;
  }
}

template<typename Type>
typename max_heap<Type>::reference
max_heap<Type>::maximum() const { return rep[0]; }

template<typename Type>
void max_heap<Type>::remove(iterator elem) {
  std::swap(*elem, *(end() - 1));
  rep.resize(size() - 1);
  if (size() > 0)
    bubble_down(begin());
}

template<typename Type>
void max_heap<Type>::remove_maximum() {
  remove(begin());
}

template<typename Type>
typename max_heap<Type>::iterator
max_heap<Type>::parent_of(iterator child) {
  // parent = floor((i - 1) / 2)
  const auto idx = std::distance(begin(), child);
  return begin() + (idx - 1) / 2;
}

template<typename Type>
typename max_heap<Type>::iterator
max_heap<Type>::left_child_of(iterator parent) {
  // left_child = 2i + 1
  const auto idx = std::distance(begin(), parent);
  return begin() + (2 * idx) + 1;
}

template<typename Type>
typename max_heap<Type>::iterator
max_heap<Type>::right_child_of(iterator parent) {
  // right_child = 2i + 2
  const auto idx = std::distance(begin(), parent);
  return begin() + (2 * idx) + 2;
}

template<typename Type>
void max_heap<Type>::bubble_up(iterator elem) {
  auto child = elem;
  auto parent = parent_of(child);

  // bubble up
  while (child != parent && *child > *parent) {
    std::swap(*child, *parent);
    child = parent;
    parent = parent_of(parent);
  }
}

template<typename Type>
void max_heap<Type>::bubble_down(iterator elem) {
  bubble_down(elem, end());
}

template<typename Type>
void max_heap<Type>::bubble_down(iterator elem, iterator last) {
  auto parent = elem;
  auto left_child = left_child_of(parent);
  auto right_child = right_child_of(parent);

  // stop at last
  while (left_child < last || right_child < last) {
    // find maximum of parent, left_child, right_child
    auto max = parent;
    if (left_child < last)
      if (*max < *left_child)
        max = left_child;
    if (right_child < last)
      if (*max < *right_child)
        max = right_child;

    // max_heap property fixed
    if (parent == max) break;

    // swap with the greater child
    std::swap(*parent, *max);
    parent = max;
    left_child = left_child_of(parent);
    right_child = right_child_of(parent);
  }
}

template<typename Type>
void max_heap<Type>::insert(value_type value) {
  rep.push_back(value);
  bubble_up(end() - 1);
}

template<typename Type>
std::ostream& operator<<(std::ostream& out, const max_heap<Type>& max_heap) {
  // output contents of max_heap
  if (max_heap.size() > 0) {
    std::cout << *max_heap.begin();
    for (auto i = max_heap.begin() + 1; i < max_heap.end(); ++i)
      std::cout << ' ' << *i;
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
      const int n = std::stoi(line);
      arr.push_back(n);
    } catch (...) {
      // ignore for now
      continue;
    }
  }

  // linear time to build max_heap
  max_heap<int> h(arr.begin(), arr.end());
  std::cout << "h before: " << h << '\n';
  h.sort();
  std::cout << "h sorted: " << h << '\n';
  h.build();
  h.insert(22);
  std::cout << "h inserted: " << h << '\n';
}
