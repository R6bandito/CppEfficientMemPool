#include <iostream>
#include "MemoryPool.h"
#include "MemoryPool.cc"

int main() {
  // MemoryPool<int> Pool_int;
  // MemoryPool<double> Pool_double;
  //MemoryPool<char> Pool_char(Pool_int);  //Error!Cant Copy
  //MemoryPool<double> Pool_char = std::move(Pool_int); //Error.
  //MemoryPool<double> Pool_double2 = Pool_double;  //Error.Cant Copy.
  // MemoryPool<int> Pool_int2(std::move(Pool_int));  //allow
  // MemoryPool<double> Pool_double2 = std::move(Pool_double);  //allow
  MemoryPool<int> Pool_int3;
  Pool_int3.allocNumSet(128);
  int *ptr_to_int = Pool_int3.allocate();
  return 0;
}