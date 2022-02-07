#include<vector>
#include<iostream>
#include <iterator>
/*
template<typename node, typename T>
class _iterator{ //forward iterator
    node* current;
public:
    using value_type = T;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;
    
    explicit _iterator(node x): current{&x} {}
    
    reference operator*() const { return current->value; }
    
    pointer operator->(){ return &**this; }

    _iterator& operator++(){
       // current = reinterpret_cast<node*>(current->value);
        current = reinterpret_cast<node*>(&current->value);
        return *this;
    }
    
    _iterator& operator++(int){
        auto tmp = *this;
        ++(*this);
        return tmp;   
    }

    friend bool operator==(const _iterator& x, const _iterator& y){
        return x.current == y.current;
    }

    friend bool operator!=(const _iterator& x, const _iterator& y){
        return !(x==y);
    }
};        

template<typename N, typename T>
class _iterator{ //forward iterator
     N current;
public:
    using value_type = T;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    explicit _iterator(N x): current{x} {}

    reference operator*() const { return pool[current-1].value; }

    pointer operator->(){ return &**this; }

    _iterator& operator++(){
        current = pool[current-1].next;
        return *this;
    }

    _iterator& operator++(int){
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    friend bool operator==(const _iterator& x, const _iterator& y){
        return x.current == y.current;
    }

    friend bool operator!=(const _iterator& x, const _iterator& y){
        return !(x==y);
    }
};*/



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

    node_t* next_node (node_t* prev) { return &pool[ prev->next -1 ];}
        std::cout << current->value << " " << current->next<< std::endl;    
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

    template <typename O>
    class _iterator{
        stack_type current;
        public:
        using value_type = O;
        using reference = value_type&;
        using pointer = value_type*;
        using difference_type = stack_type;
        using iterator_category = std::forward_iterator_tag;
         
        explicit _iterator(stack_type x): current{x} {}
        
        reference operator*() const { return value(current); }
        
        pointer operator->(){ return &**this; }
    
        _iterator& operator++(){
            current = next(current);
            return *this;
        }
        
        _iterator& operator++(int){
            auto tmp = *this;
            ++(*this);
            return tmp;   
        }
     
        friend bool operator==(const _iterator& x, const _iterator& y){ return x.current == y.current; }
        friend bool operator!=(const _iterator& x, const _iterator& y){ return !(x==y); }
    };        

    using iterator = _iterator<T>;
    using const_iterator = _iterator<const T>;

    iterator begin(stack_type x) { return iterator{x}; }
    iterator end(stack_type ) { return iterator{end()}; } // this is not a typo

    //const_iterator begin(stack_type x) const { return const_iterator{node(x)}; }
    //const_iterator end(stack_type ) const{ return const_iterator{node(end())}; }
      
    //const_iterator cbegin(stack_type x) const { return const_iterator{node(x)}; }
    //const_iterator cend(stack_type ) const { return const_iterator{node(end())}; }

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
/*
    stack_type push(const T& val, stack_type head){ // meglio implementare fuori forse perché sono grandi
        if (!empty(free_nodes)){
            // liberiamo free_nodes
            auto new_head = free_nodes;		
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
   stack_type push(T&& val, stack_type head){ // std::move somewhere?
        if (!empty(free_nodes)){
            // liberiamo free_nodes
            auto new_head = free_nodes;		
            free_nodes = node(free_nodes).next;	
            // push nel nuovo stack	
            node(new_head) = node_t{val,head}; // move assignement + ctor
            // node(new_head) = node_t{std::move(val),std::move(head)};
            //  		node(new_head).next = head;
            //  		node(new_head).value = val;
        return new_head;
        }
        else {
            pool.push_back(node_t{val,head}); 
            // pool.push_back(node_t{std::move(val),std::move(head)});
            return stack_type(pool.size());
        }
    }
*/
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
    
    friend node_t& operator++(node_t& x){
        return node(x.next);
    }
};

/*
template <typename T, typename N>
template <typename O>
class stack_pool<T,N>::_iterator{
    using index = typename stack_pool<T,N>::stack_type;
    index current;
    public:
    using value_type = O;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = index;
    using iterator_category = std::forward_iterator_tag;
    
    explicit _iterator(index x): current{x} {}
    
    reference operator*() const {    
        return value(current);
     }
    
    pointer operator->(){ return &**this; }

    _iterator& operator++(){
        current = next(current);
        return *this;
    }
    
    _iterator& operator++(int){
        auto tmp = *this;
        ++(*this);
        return tmp;   
    }

    friend bool operator==(const _iterator& x, const _iterator& y){
        return x.current == y.current;
    }

    friend bool operator!=(const _iterator& x, const _iterator& y){
        return !(x==y);
    }
};        



template <typename T, typename N>
template <typename O>
class stack_pool<T,N>::_iterator{
    using nodenode = typename stack_pool<T,N>::node_t;
    nodenode* current;
    public:
    using value_type = O;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;
    
    explicit _iterator(nodenode* x): current{x} {}
    
    reference operator*() const { return current->value; }
    
    pointer operator->(){ return &**this; }

    _iterator& operator++(){
        current = &*current++;
        return *this;
    }
    
    _iterator& operator++(int){
        auto tmp = *this;
        ++(*this);
        return tmp;   
    }

    friend bool operator==(const _iterator& x, const _iterator& y){
        return x.current == y.current;
    }

    friend bool operator!=(const _iterator& x, const _iterator& y){
        return !(x==y);
    }
};        
*/

