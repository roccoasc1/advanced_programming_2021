/**
	* Implement a pool of blazingly fast stacks
	*
	* authors: Rocco Ascone, Simone Pozzi
	*/

// DUBBI: 
// 1)dire che adress non è davvero l'adress?
// 2)scrivere qualcosa sui (move copy) ctors di node?

#ifndef __SP__
#define __SP__

#include <iostream>
#include <iterator>
#include <vector>

template <typename T, typename N = std::size_t>
class stack_pool {
  struct node_t {
 	/**
 		* Define a single node of the pool. 
 		* It contains a T value and an index N
 		* referring to the "adress" of the next node.
 		* Adress 0 is referred as end.
 		*/
    T value;
    N next;
    
    node_t(const T& v, N n):value{v}, next{n} {}
    node_t(T&& v, N n):value{std::move(v)}, next{n} {}
  };
	/** 
		* The pool stores each node in a vector.
		* The adress of a node is 1 + index where
		* the node is stored in the vector.
		* A stack in considered as the adress of
		* its first element.
		*/
  std::vector<node_t> pool;
  
  using stack_type = N;
  using value_type = T;
  using size_type =  typename std::vector<node_t>::size_type;
                          
	/** A stack of free nodes (i.e. available positions
		* in the vector) is always present in the pool.
		* At the beginning it is empty
		*/                      
  stack_type free_nodes = end();

	// to obtain the node from the adress (stack_type):
  node_t& node(stack_type x) noexcept { return pool[x - 1]; }
  const node_t& node(stack_type x) const noexcept { return pool[x - 1]; }

	// ausiliary function for push. Implemented outside the class
  template <typename V>
  stack_type _push(V&& val, stack_type head);

  stack_type release_free_nodes() noexcept { 
  /**
  	* Change the head of free nodes to second element
  	* and returns the first one.
  	* We use this function only when free_nodes is not empty
  	*/
    auto new_head = free_nodes;
    free_nodes = next(free_nodes);
    return new_head;
  }

  void push_free_nodes(stack_type x) noexcept {
 	/** 
 		* Add node in position x to free_nodes
 		*/
    next(x) = free_nodes;
    free_nodes = x;
  }

 public:
  stack_pool() noexcept = default;
  
  // Reserves n nodes in the pool
  explicit stack_pool(size_type n) : pool{} { reserve(n); } 

  template <typename O>
  class _iterator; // Declared in "iterator.hpp"

  using iterator = _iterator<T>;
  using const_iterator = _iterator<const T>;

  iterator begin(stack_type x) { return iterator{x, *this}; }
  iterator end(stack_type) { return iterator{end(), *this}; }

  const_iterator begin(stack_type x) const { return const_iterator{x, *this}; }
  const_iterator end(stack_type) const { return const_iterator{end(), *this}; }

  const_iterator cbegin(stack_type x) const { return const_iterator{x, *this}; }
  const_iterator cend(stack_type) const { return const_iterator{end(), *this}; }

  stack_type new_stack() noexcept { return end(); }  // Return an empty stack

  void reserve(size_type n) { pool.reserve(n); }  // Reserve n nodes in the pool
  size_type capacity() const noexcept { return pool.capacity(); }  // Capacity of the pool

  bool empty(stack_type x) const noexcept { return x == end(); } // True if stack is empty

  stack_type end() const noexcept { return stack_type(0); }

  T& value(stack_type x) noexcept { return node(x).value; }
  const T& value(stack_type x) const noexcept { return node(x).value; }

  stack_type& next(stack_type x) noexcept { return node(x).next; }
  const stack_type& next(stack_type x) const noexcept { return node(x).next; }

  stack_type push(const T& val, stack_type head) { return _push(val, head); }
  stack_type push(T&& val, stack_type head) { return _push(std::move(val), head); }

  stack_type pop(stack_type x) noexcept {  
  /** 
  	* Delete the first node of a stack and
  	* returns the stack itself.
  	*/
    auto new_head = next(x);
    push_free_nodes(x);
    return new_head;
  }

  stack_type free_stack(stack_type x) noexcept {  
  /** 
  	* Free an entire stack.
  	* Returns an empty stack (i.e. end() )
  	*/
    while (!empty(x))
      x = pop(x);
    return x;
  }

  void print_pool() const noexcept {
  /**
  	* Print the content of the whole poole.
  	* Used for debugging.
  	*/
    std::cout << "value\tnext\n";
    for (const auto x : pool)
      std::cout << x.value << "\t" << x.next << std::endl;
  }
};


template <typename T, typename N>
template <typename V>
N stack_pool<T, N>::_push(V&& val, stack_type head) {
	/**
		* Ausiliary function for push to deal both with
		* r-value ref and l-value ref.
		* If no free nodes are available it emplace_back
		* a new element in the vector. Otherwise it release
		* the first free node and "create" there the node.
		*/
  if (empty(free_nodes)) {
    pool.emplace_back(std::forward<V>(val), head);
    return stack_type(pool.size());
  }
  auto new_head = release_free_nodes();
  node(new_head) = node_t{std::forward<V>(val), head};
  return new_head;
}

#include"iterator.hpp"

#endif
