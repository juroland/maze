#ifndef UTILITY_H
#define UTILITY_H

#include <memory>

// excerpt from llvm
template <typename IteratorT>
class range {
  IteratorT begin_iterator, end_iterator;

public:
  template <typename Container>
  range(Container &&c)
      : begin_iterator(c.begin()), end_iterator(c.end()) {}
  range(IteratorT begin_iterator, IteratorT end_iterator)
      : begin_iterator(std::move(begin_iterator)),
        end_iterator(std::move(end_iterator)) {}

  IteratorT begin() const { return begin_iterator; }
  IteratorT end() const { return end_iterator; }
};

template <class T> range<T> make_range(T x, T y) {
  return range<T>(std::move(x), std::move(y));
}

template <typename T> range<T> make_range(std::pair<T, T> p) {
  return range<T>(std::move(p.first), std::move(p.second));
}

template<typename T>
range<decltype(begin(std::declval<T>()))> drop_begin(T &&t, int n) {
  return make_range(std::next(begin(t), n), end(t));
}

#endif // UTILITY_H
