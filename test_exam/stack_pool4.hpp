#pragma once
#include <iostream>
#include <iterator>
#include <vector>

template <typename T, typename N = std::size_t>
class stack_pool {
  struct node_t {
    T value;
    N next;
    node_t(): value{}, next{}{ std::cout<<"default ctor"<<std::endl; }
    //node_t(node_t&& node) = default;
    //node_t(const node_t& node) = default;
    node_t& operator= (node_t&& node) = default;
    node_t& operator= (const node_t& node) = default;
    ~node_t() = default;
    node_t(node_t&& node):value{std::move(node.value)}, next{std::move(node.next)}{std::cout<<"move ctor"<<std::endl;}
    node_t(const node_t& node): value{node.value}, next{node.next}{std::cout<<"copy ctor"<<std::endl;}
    
    node_t(const T& v, N n):value{v}, next{n} {std::cout<<"value copy ctor"<<std::endl;};
    node_t(T&& v, N n):value{std::move(v)}, next{n} {std::cout<<"value move ctor"<<std::endl;}
  };

  std::vector<node_t> pool;
  using stack_type = N;
  using value_type = T;
  using size_type = typename std::vector<node_t>::size_type;
                                                
  stack_type free_nodes = end();                // at the beginning, it is empty

  node_t& node(stack_type x) noexcept { return pool[x - 1]; }
  const node_t& node(stack_type x) const noexcept { return pool[x - 1]; }

  template <typename V>
  stack_type _push(V&& val, stack_type head);

  stack_type release_free_nodes() noexcept { // here free_nodes is not empty
    auto new_head = free_nodes;
    free_nodes = next(free_nodes);
    return new_head;
  }

  void push_free_nodes(stack_type x) noexcept {
    next(x) = free_nodes;
    free_nodes = x;
  }

 public:
  stack_pool() noexcept = default;
  explicit stack_pool(size_type n) : pool{} { reserve(n); } 

  template <typename O>
  class _iterator;

  using iterator = _iterator<T>;
  using const_iterator = _iterator<const T>;

  iterator begin(stack_type x) { return iterator{x, *this}; }
  iterator end(stack_type) { return iterator{end(), *this}; }

  const_iterator begin(stack_type x) const { return const_iterator{x, *this}; }
  const_iterator end(stack_type) const { return const_iterator{end(), *this}; }

  const_iterator cbegin(stack_type x) const { return const_iterator{x, *this}; }
  const_iterator cend(stack_type) const { return const_iterator{end(), *this}; }

  stack_type new_stack() noexcept { return end(); }  // return an empty stack

  void reserve(size_type n) { pool.reserve(n); }  // reserve n nodes in the pool
  size_type capacity() const noexcept {
    return pool.capacity();
  }  // the capacity of the pool

  bool empty(stack_type x) const noexcept { return x == end(); }

  stack_type end() const noexcept { return stack_type(0); }

  T& value(stack_type x) noexcept { return node(x).value; }
  const T& value(stack_type x) const noexcept { return node(x).value; }

  stack_type& next(stack_type x) noexcept { return node(x).next; }
  const stack_type& next(stack_type x) const noexcept { return node(x).next; }

  stack_type push(const T& val, stack_type head) { return _push(val, head); }
  stack_type push(T&& val, stack_type head) {
    return _push(std::move(val), head);
  }

  stack_type pop(stack_type x) noexcept {  // delete first node
    auto new_head = next(x);
    push_free_nodes(x);
    return new_head;
  }

  stack_type free_stack(stack_type x) noexcept {  // free entire stack
    while (!empty(x))
      x = pop(x);
    return x;
  }

  void print_pool() const noexcept {
    std::cout << "value\tnext\n";
    for (const auto x : pool)
      std::cout << x.value << "\t" << x.next << std::endl;
  }
};

template <typename T, typename N>
template <typename O>
class stack_pool<T, N>::_iterator {
  stack_type current;
  stack_pool<T, N>* sp_ptr;

 public:
  using value_type = O;
  using reference = value_type&;
  using pointer = value_type*;
  using difference_type = stack_type;
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

template <typename T, typename N>
template <typename V>
N stack_pool<T, N>::_push(V&& val, N head) {
  if (empty(free_nodes)) { 
    // no free_nodes available => push_back new element in the vector
    pool.emplace_back(std::forward<V>(val), head); 
    return stack_type(pool.size());
  }
  auto new_head = release_free_nodes();
  node(new_head) = node_t{std::forward<V>(val), head};
  return new_head;
}
