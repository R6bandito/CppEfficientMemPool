#include <iostream>
#include <chrono>
#include "MemoryPool.h"
#include "MemoryPool.cc"

void func_new() {
  std::cout << "normal cost" << std::endl;
  auto start = std::chrono::high_resolution_clock::now();
  for(int i = 0; i < 5000; ++i) {
    int *ptr_to_int = new int(0);
    delete ptr_to_int;
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "time cost: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl; 
}

void func_pool() {
  std::cout << "Pool cost" << std::endl;
  auto start = std::chrono::high_resolution_clock::now();
  MemoryPool<int> Pool_int;
  for(int i = 0; i < 5000; ++i) {
    int *ptr_to_int = Pool_int.allocate();
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "time cost: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
}
int main() {
  // MemoryPool<int> Pool_int;
  // MemoryPool<double> Pool_double;
  //MemoryPool<char> Pool_char(Pool_int);  //Error!Cant Copy
  //MemoryPool<double> Pool_char = std::move(Pool_int); //Error.
  //MemoryPool<double> Pool_double2 = Pool_double;  //Error.Cant Copy.
  // MemoryPool<int> Pool_int2(std::move(Pool_int));  //allow
  // MemoryPool<double> Pool_double2 = std::move(Pool_double);  //allow
  // MemoryPool<int> Pool_int3;
  // Pool_int3.allocNumSet(128);
  // int *ptr_to_int = Pool_int3.allocate();
  // MemoryPool<int> Pool_int;
  // int *ptr_to_int = Pool_int.allocate();
  // Pool_int.deallocate(ptr_to_int);
  func_new();
  func_pool();
  return 0;
}