#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <functional>

template<typename Type, typename Compare>
class heap {
public:
  using value_type = Type;
  using reference = Type&;
  using const_reference = const Type&;
  using iterator = typename std::vector<Type>::iterator;
  using const_iterator = typename std::vector<Type>::const_iterator;
  using difference_type = typename std::vector<Type>::difference_type;
  using size_type = typename std::vector<Type>::size_type;
  using value_compare = Compare;

  heap();
  ~heap() = default;
  heap(iterator begin, iterator end);
  heap(const heap<Type, Compare>& other);
  heap(heap<Type, Compare>&& other);

  iterator begin();
  const_iterator begin() const;
  const_iterator cbegin() const;
  iterator end();
  const_iterator end() const;
  const_iterator cend() const;

  reference operator=(heap<Type, Compare> other);
  reference operator=(heap<Type, Compare>&& other);
  bool operator==(const heap<Type, Compare>& other);
  bool operator!=(const heap<Type, Compare>& other);
  void swap(heap& other);
  template<typename T, typename C>
  friend void swap(heap<T, C>& lhs, heap<T, C>& rhs);
  size_type size() const;
  size_type max_size() const;
  bool empty() const;
  
  void sort();
  void insert(value_type value);
  void remove(iterator elem);
  void remove_maximum();
  reference maximum();
  const_reference maximum() const;

  // build tree in linear time
  void build();
  
private:
  iterator parent_of(iterator child);
  iterator left_child_of(iterator parent);
  iterator right_child_of(iterator parent);
  void bubble_up(iterator elem);
  void bubble_down(iterator elem);
  void bubble_down(iterator elem, iterator last);
  
  std::vector<value_type> _rep;
  size_type _size = 0;
  Compare comp;
};

template<typename Type, typename Compare>
heap<Type, Compare>::heap() : comp(Compare()) {}

template<typename Type, typename Compare>
heap<Type, Compare>::heap(iterator begin, iterator end)
  : _rep(begin, end), comp(Compare()) {
  _size = _rep.size();  
  build();
}

template<typename Type, typename Compare>
heap<Type, Compare>::heap(const heap<Type, Compare>& other)
  : _rep(other._rep), _size(other._size) { }

template<typename Type, typename Compare>
heap<Type, Compare>::heap(heap<Type, Compare>&& other)
  : _rep(std::move(other._rep)), _size(other._size) {}

template<typename Type, typename Compare>
typename heap<Type, Compare>::iterator
heap<Type, Compare>::begin() { return _rep.begin(); }

template<typename Type, typename Compare>
typename heap<Type, Compare>::const_iterator
heap<Type, Compare>::begin() const { return _rep.begin(); }

template<typename Type, typename Compare>
typename heap<Type, Compare>::const_iterator
heap<Type, Compare>::cbegin() const { return _rep.begin(); }

template<typename Type, typename Compare>
typename heap<Type, Compare>::iterator
heap<Type, Compare>::end() { return begin() + _size; }

template<typename Type, typename Compare>
typename heap<Type, Compare>::const_iterator
heap<Type, Compare>::end() const { return begin() + _size; }

template<typename Type, typename Compare>
typename heap<Type, Compare>::const_iterator
heap<Type, Compare>::cend() const { return begin() + _size; }

template<typename Type, typename Compare>
typename heap<Type, Compare>::reference
heap<Type, Compare>::operator=(heap<Type, Compare> other) {
  // copy-swap
  std::swap(_rep, other._rep);
  std::swap(_size, other._size);
  return *this;
}

template<typename Type, typename Compare>
typename heap<Type, Compare>::reference
heap<Type, Compare>::operator=(heap<Type, Compare>&& other) {
  // copy-swap
  std::swap(_rep, other._rep);
  std::swap(_size, other._size);
  return *this;
}

template<typename Type, typename Compare>
bool heap<Type, Compare>::operator==(const heap<Type, Compare>& other) {
  return std::equal(begin(), end(), other.begin(), other.end());
}

template<typename Type, typename Compare>
bool heap<Type, Compare>::operator!=(const heap<Type, Compare>& other) {
  return !operator==(other);
}

template<typename Type, typename Compare>
void heap<Type, Compare>::swap(heap<Type, Compare>& other) {
  std::swap(_rep, other._rep);
  std::swap(_size, other._size);
}

template<typename Type, typename Compare>
void swap(heap<Type, Compare>& lhs, heap<Type, Compare> rhs) {
  lhs.swap(rhs);
}

template<typename Type, typename Compare>
typename heap<Type, Compare>::size_type
heap<Type, Compare>::size() const { return _size; }

template<typename Type, typename Compare>
typename heap<Type, Compare>::size_type
heap<Type, Compare>::max_size() const { return _rep.max_size(); }

template<typename Type, typename Compare>
bool heap<Type, Compare>::empty() const { return _size == 0; }

template<typename Type, typename Compare>
void heap<Type, Compare>::build() {
  // skip leaf nodes  
  const auto n = begin() + (size() / 2) - 1;
  for (auto i = n; i >= begin(); --i)
    bubble_down(i);
}

template<typename Type, typename Compare>
void heap<Type, Compare>::sort() {
  auto iter = end() - 1;

  while (iter >= begin()) {
    using std::swap;
    swap(*begin(), *iter);
    // bubble root down but ignore elements past iter
    bubble_down(begin(), iter);
    --iter;
  }
}

template<typename Type, typename Compare>
typename heap<Type, Compare>::reference
heap<Type, Compare>::maximum() { return *begin(); }

template<typename Type, typename Compare>
typename heap<Type, Compare>::const_reference
heap<Type, Compare>::maximum() const { return *begin(); }

template<typename Type, typename Compare>
void heap<Type, Compare>::remove(iterator elem) {
  using std::swap;
  swap(*elem, *(end() - 1));
  --_size;
  if (size() > 0)
    bubble_down(begin());
}

template<typename Type, typename Compare>
void heap<Type, Compare>::remove_maximum() {
  remove(begin());
}

template<typename Type, typename Compare>
typename heap<Type, Compare>::iterator
heap<Type, Compare>::parent_of(iterator child) {
  // parent = floor((i - 1) / 2)
  const auto idx = child - begin();
  return begin() + (idx - 1) / 2;
}

template<typename Type, typename Compare>
typename heap<Type, Compare>::iterator
heap<Type, Compare>::left_child_of(iterator parent) {
  // left_child = 2i + 1
  const auto idx = parent - begin();
  return begin() + (2 * idx) + 1;
}

template<typename Type, typename Compare>
typename heap<Type, Compare>::iterator
heap<Type, Compare>::right_child_of(iterator parent) {
  // right_child = 2i + 2
  const auto idx = parent - begin();
  return begin() + (2 * idx) + 2;
}

template<typename Type, typename Compare>
void heap<Type, Compare>::bubble_up(iterator elem) {
  auto child = elem;
  auto parent = parent_of(child);

  // bubble up
  while (child != parent && comp(*parent, *child)) {
    using std::swap;
    swap(*child, *parent);
    child = parent;
    parent = parent_of(parent);
  }
}

template<typename Type, typename Compare>
void heap<Type, Compare>::bubble_down(iterator elem) {
  bubble_down(elem, end());
}

template<typename Type, typename Compare>
void heap<Type, Compare>::bubble_down(iterator elem, iterator last) {
  auto parent = elem;
  auto left_child = left_child_of(parent);
  auto right_child = right_child_of(parent);

  // stop at last
  while (left_child < last || right_child < last) {
    // find maximum of parent, left_child, right_child
    auto max = parent;
    if (left_child < last)
      if (comp(*max, *left_child))
        max = left_child;
    if (right_child < last)
      if (comp(*max, *right_child))
        max = right_child;

    // heap property fixed
    if (parent == max) break;

    // swap with the greater child
    using std::swap;
    swap(*parent, *max);
    parent = max;
    left_child = left_child_of(parent);
    right_child = right_child_of(parent);
  }
}

template<typename Type, typename Compare>
void heap<Type, Compare>::insert(value_type value) {
  if (_size < _rep.size()) {
    _rep[_size++] = std::move(value);
  } else {
    _rep.push_back(std::move(value));
    ++_size;
  }
  
  bubble_up(end() - 1);
}

template<typename Type, typename Compare>
std::ostream& operator<<(std::ostream& out, const heap<Type, Compare>& heap) {
  // output contents of heap
  if (heap.size() > 0) {
    std::cout << *heap.begin();
    for (auto i = heap.begin() + 1; i < heap.end(); ++i)
      std::cout << ' ' << *i;
  }
  
  return out;
}

template<typename Type>
using max_heap = heap<Type, std::less<Type>>;

template<typename Type>
using min_heap = heap<Type, std::greater<Type>>;

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

  // linear time to build heap
  max_heap<int> h(arr.begin(), arr.end());
  std::cout << "h before: " << h << '\n';
  h.sort();
  std::cout << "h sorted: " << h << '\n';
}
