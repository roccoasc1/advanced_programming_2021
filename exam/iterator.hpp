#ifndef __ITERATOR__
#define __ITERATOR__

template <typename T, typename N>
template <typename O>
class stack_pool<T, N>::_iterator {
  /**
   * Implement an iterator class for our stack_pool.
   * Templates T,N are the templates of the pool.
   * Template O refers to const-ness.
   */
  stack_type current;
  stack_pool<T, N>* sp_ptr;

 public:
  using value_type = O;
  using reference = value_type&;
  using pointer = value_type*;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  _iterator(stack_type x, stack_pool<T, N>& sp) : current{x}, sp_ptr{&sp} {}

  reference operator*() const { return sp_ptr->value(current); }

  pointer operator->() { return &**this; }

  _iterator& operator++() {
    current = sp_ptr->next(current);
    return *this;
  }

  _iterator& operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  friend bool operator==(const _iterator& x, const _iterator& y) {
    return (x.current == y.current) && (x.sp_ptr == y.sp_ptr);
  }

  friend bool operator!=(const _iterator& x, const _iterator& y) {
    return !(x == y);
  }

};

#endif
