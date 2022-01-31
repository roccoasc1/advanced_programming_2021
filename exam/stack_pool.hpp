#include<vector>
#include<iostream>

template <typename T, typename N = std::size_t>
class stack_pool{
  struct node_t{
    T value;
    N next;
  };
  std::vector<node_t> pool;
  using stack_type = N;
  using value_type = T;
  using size_type = typename std::vector<node_t>::size_type; 				// size_type come size_type di std::vectors
  stack_type free_nodes =end(); // at the beginning, it is empty			// = end 
  
  node_t& node(stack_type x) noexcept { return pool[x-1]; }
  const node_t& node(stack_type x) const noexcept { return pool[x-1]; }

  public:
  stack_pool() = default; 													//mettere = default
  explicit stack_pool(size_type n) :pool{} { reserve(n); }// reserve n nodes in the pool
    
//  using iterator = ...;
//  using const_iterator = ...;

//  iterator begin(stack_type x);
//  iterator end(stack_type ); // this is not a typo
    
//  const_iterator begin(stack_type x) const;
//  const_iterator end(stack_type ) const; 
//  
//  const_iterator cbegin(stack_type x) const;
//  const_iterator cend(stack_type ) const;
    
  stack_type new_stack() { return end(); } // return an empty stack

  void reserve(size_type n){pool.reserve(n);} // reserve n nodes in the pool
  size_type capacity() const noexcept {return pool.capacity(); } // the capacity of the pool

  bool empty(stack_type x) const noexcept { return x == end(); } // sì se vuoto

  stack_type end() const noexcept { return stack_type(0); }

  T& value(stack_type x){ return node(x).value; }
  const T& value(stack_type x) const { return node(x).value; }

  stack_type& next(stack_type x) {return node(x).next;}
  const stack_type& next(stack_type x) const {return node(x).next;}

  stack_type push(const T& val, stack_type head){ // meglio implementare fuori forse perché sono grandi
    if (!empty(free_nodes)){
      // liberiamo free_nodes
      auto new_head  = free_nodes;		
      free_nodes = node(free_nodes).next;	
      // push nel nuovo stack	
      node(new_head) = node_t{val,head}; // move assignement + ctor
      return new_head;
    }
    else {
      pool.push_back(node_t{val,head}); // con push_back va in fondo
      return stack_type(pool.size());
    }
  }
  stack_type push(T&& val, stack_type head){
  	if (!empty(free_nodes)){
  		// liberiamo free_nodes
  		auto new_head  = free_nodes;		
  		free_nodes = node(free_nodes).next;	
  		// push nel nuovo stack	
  		node(new_head) = node_t{val,head}; // move assignement + ctor
//  		node(new_head).next = head;
//  		node(new_head).value = val;
  		return new_head;
  	}
  	else {
//		pool[pool.size()-1] = node_t{val,head};
  		pool.push_back(node_t{val,head}); 
  		return stack_type(pool.size());
  	}
  }
  
  stack_type pop(stack_type x){ // delete first node
  	auto new_head =node(x).next;
  	node(x).next = free_nodes;
  	free_nodes = x;
  	return new_head;  
  }

  stack_type free_stack(stack_type x){ // free entire stack
  	while (!empty(x))
  		x = pop(x);
  	return x;
  }
  
  void print_pool() const noexcept {
  	std::cout << "value\tnext\n";
  	for ( const auto x : pool)
  		std::cout << x.value << "\t" << x.next << std::endl;
  }
    
};


