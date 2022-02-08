#include <vector>
#include <iostream>
#include <iterator>

template <typename T, typename N = std::size_t>
class stack_pool{
	struct node_t{
    	T value;
    	N next;
 
        //explicit node_t(const T& x): value{x} {}
        //node_t(const T& x, N y): value{x}, next{y}{}        
        //explicit node_t(T&& x): value{std::move(x)} {}
        //node_t(T&& x, N y): value{std::move(x)}, next{y} {}
        //node_t (T val, N nex): value{val}, next{nex} {}
        //node_t (N n): node_t{node(n)} {}
        //node_t(node_t&&) = default;
    };
 
    std::vector<node_t> pool;
    using stack_type = N;
	using value_type = T;
	using size_type = typename std::vector<node_t>::size_type; 				// size_type come size_type di std::vectors
    stack_type free_nodes = end(); // at the beginning, it is empty			     
    
    node_t& node(stack_type x) noexcept { return pool[x-1]; }
    const node_t& node(stack_type x) const noexcept { return pool[x-1]; }

    template<typename V>
    stack_type _push(V&& val, stack_type head){
        if (!empty(free_nodes)){
            auto new_head = free_nodes;
            free_nodes = node(free_nodes).next;
            node(new_head) = node_t{std::forward<V>(val), head}; // move assignement + ctor
            return new_head;
        }
        pool.push_back(node_t{std::forward<V>(val),head}); // con push_back va in fondo
        return stack_type(pool.size());
    }
    
    public:
    stack_pool() = default;
    explicit stack_pool(size_type n) :pool{} { reserve(n); }// reserve n nodes in the pool
    //stack_pool(const stack_pool& ) = default;
    //stack_pool(stack_pool&& ) = default;
    //stack_pool& operator=(stack_pool&& ) = default;
    //stack_pool& operator=(const stack_pool& ) = default;
    
    template <typename O>
	class _iterator{
		stack_type current;
		std::vector<node_t>& ps;
		public:
		using value_type = O;
		using reference = value_type&;
		using pointer = value_type*;
		using difference_type = stack_type;
		using iterator_category = std::forward_iterator_tag;
		 
		_iterator(stack_type x, std::vector<node_t> pp): current{x}, ps{pp} {std::cout << "ctor\n";}
		//_iterator(const _iterator& x): current{x.current}, sp{x.sp}{}
		//_iterator(_iterator&& x ): current{std::move(x.current)}, sp{x.sp}{}
		//_iterator() = default;
		//_iterator(_iterator&& ) = default;
		//_iterator(const _iterator& ) = default;
		//_iterator& operator=(_iterator&& )=default;
		//_iterator& operator=(const _iterator& ) = default;
		~_iterator() noexcept { std::cout << "dtor\n"; }

		reference operator*() const { 
			//std::cout << ps[current-1].value << std::endl;
			return ps[current-1].value; 
		} 

		pointer operator->(){ return &**this; }

		_iterator& operator++(){
			std::cout << ps[current-1].next << std::endl;
			current = ps[current-1].next;
			return *this;
    	}
    
		_iterator& operator++(int){ //ok sempre così
		    auto tmp = *this;
		    ++(*this);
		    return tmp;   
		}
	 
		friend bool operator==(const _iterator& x, const _iterator& y){ return x.current == y.current; }
		friend bool operator!=(const _iterator& x, const _iterator& y){ return !(x==y); }
	};        


    using iterator = _iterator<T>;
    using const_iterator = _iterator<const T>;

    iterator begin(stack_type x) { return iterator(x, pool); }
    iterator end(stack_type ) { return iterator(end(),pool); } // this is not a typo

    //const_iterator begin(stack_type x) const { return const_iterator(x, pool); }
    //const_iterator end(stack_type ) const{ return const_iterator(end(), pool); }
      
    //const_iterator cbegin(stack_type x) const { return const_iterator(x, pool); }
    //const_iterator cend(stack_type ) const { return const_iterator(end(),pool); }

    stack_type new_stack() { return end(); } // return an empty stack

    void reserve(size_type n){ pool.reserve(n); } // reserve n nodes in the pool
    size_type capacity() const noexcept { return pool.capacity(); } // the capacity of the pool

    bool empty(stack_type x) const noexcept { return x == end(); } // sì se vuoto

    stack_type end() const noexcept { return stack_type(0); }

    T& value(stack_type x){ return node(x).value; }
    const T& value(stack_type x) const { return node(x).value; }

    stack_type& next(stack_type x) { return node(x).next; }
    const stack_type& next(stack_type x) const { return node(x).next; }

    stack_type push(const T& val, stack_type head){ return _push(val, head); }
    stack_type push(T&& val, stack_type head){ return _push(std::move(val), head); }

    stack_type pop(stack_type x){ // delete first node
        auto new_head = node(x).next;
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
        for (const auto x : pool)
            std::cout << x.value << "\t" << x.next << std::endl;
    }
  
    friend node_t& operator++(node_t& x){ return node(x.next); }
};




