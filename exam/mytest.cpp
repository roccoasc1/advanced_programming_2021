#include "stack_pool.hpp"
#include<iostream>
#include <cassert>
#include <algorithm> 

template <typename T>
void f(T x);

int main(){
    {
    stack_pool<int, std::size_t> pool{};
    auto l = pool.new_stack();
    
    pool.print_pool();
    std::cout << "l == pool.end()    \t" << (l == pool.end()) << "\n"
              << "l == std::size_t(0)\t" << (l == std::size_t(0)) << std::endl;
    
    l = pool.push(42,l);
    std::cout << "l == std::size_t(1)\t" << (l == std::size_t(1)) << std::endl;
    pool.print_pool();

    pool.value(l) = 77;
    pool.print_pool();



    stack_pool<int, std::size_t> pool1{};
    auto l1 = pool1.new_stack();
    l1 = pool1.push(10,l1); // l1 == std::size_t(1)
    std::cout << "l1 == std::size_t(1)\t" << (l1 == std::size_t(1)) << std::endl; 
    pool1.print_pool();
    
    l1 = pool1.push(11,l1); // l1 == std::size_t(2) <-- later, this node will be deleted
    std::cout << "l1 == std::size_t(2)\t" << (l1 == std::size_t(2)) << std::endl;
    pool1.print_pool();

    auto l2 = pool1.new_stack();
    l2 = pool1.push(20,l2); // l2 == std::size_t(3)
    std::cout << "l2 == std::size_t(1)\t" << (l2 == std::size_t(3)) << std::endl;
    pool1.print_pool();
  
    l1 = pool1.pop(l1); // that node is deleted, so it is added to free_nodes
    pool1.print_pool();
    
    l2 = pool1.push(21,l2); // l2 == std::size_t(2)
    std::cout << "l2 == std::size_t(2)\t" << (l2 == std::size_t(2)) << std::endl;
    pool1.print_pool();
    }
    {
    stack_pool<int> pool{22};
    auto l1 = pool.new_stack();
    // credits: pi as random number generator :)
    l1 = pool.push(3, l1);
    l1 = pool.push(1, l1);
    l1 = pool.push(4, l1);
    l1 = pool.push(1, l1);
    l1 = pool.push(5, l1);
    l1 = pool.push(9, l1);
    l1 = pool.push(2, l1);
    l1 = pool.push(6, l1);
    l1 = pool.push(5, l1);
    l1 = pool.push(3, l1);
    l1 = pool.push(5, l1);

    auto l2 = pool.new_stack();
    l2 = pool.push(8, l2);
    l2 = pool.push(9, l2);
    l2 = pool.push(7, l2);
    l2 = pool.push(9, l2);
    l2 = pool.push(3, l2);
    l2 = pool.push(1, l2);
    l2 = pool.push(1, l2);
    l2 = pool.push(5, l2);
    l2 = pool.push(9, l2);
    l2 = pool.push(9, l2);
    l2 = pool.push(7, l2);

    auto M = std::max_element(pool.begin(l1), pool.end(l1));
    std::cout << *M << " == 9?: "<< (*M==9) << "\n"; 

    auto m = std::min_element(pool.begin(l2), pool.end(l2));
    std::cout << *m << " ==1? " << (*m == 1) << std::endl;

    }
    return 0; 
}
