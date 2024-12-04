#include <iostream>
#include "MemoryPool.h"

int main() {
  MemoryPool<int> Pool_int;
  MemoryPool<double> Pool_double;
  //MemoryPool<char> Pool_char(Pool_int);  //Error!Cant Copy
  //MemoryPool<double> Pool_char = std::move(Pool_int); //Error.
  //MemoryPool<double> Pool_double2 = Pool_double;  //Error.Cant Copy.
  MemoryPool<int> Pool_int2(std::move(Pool_int));  //allow
  MemoryPool<double> Pool_double2 = std::move(Pool_double);  //allow
  return 0;
}