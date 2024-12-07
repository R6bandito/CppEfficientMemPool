/* MIT License

Copyright (c) 2024 R6bandito

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

#include <climits>
#include <cstddef>
#include <vector>
#include <cassert>

#define DISALLOW_AND_ASSIGN(TypeName) \
  MemoryPool(const TypeName &) = delete; \
  MemoryPool& operator=(const TypeName &) = delete; \

template<class T> 
class MemoryPool {
 public:
  using value_type      = T;
  using pointer         = T*;
  using reference       = T&;
  using const_pointer   = const T*;
  using const_reference = const T&;
  using size_type       = size_t;
  using diff_ptr_type   = ptrdiff_t;

  template<typename U> struct rebind {
    using other = MemoryPool<U>;
  };

  MemoryPool() noexcept;
  MemoryPool(MemoryPool &&) noexcept;
  MemoryPool& operator=(MemoryPool &&) noexcept;
  ~MemoryPool();

  pointer address(reference __X) const noexcept;
  const_pointer address(const_reference __X) const noexcept;

  pointer allocate (size_type __n = 1, const void* __p = nullptr);
  void deallocate(pointer __p, size_type __n = 1);

  void allocNumSet(size_type __n);
  void alignasNumSet(size_type __n);

 private:
  template<typename Type>
  MemoryPool(const MemoryPool<Type> &) = delete;
  DISALLOW_AND_ASSIGN(MemoryPool);

  struct _Slot{
    _Slot* next;
  };
  std::vector<char *> Block_Container;
  _Slot *freelist;
  size_type Object_Size;
  size_type Object_Num;
  size_type alignas_Num;

  void Block_Alloc();
  size_type padCaculate(void *Block_ptr);
};
#endif // __MEMORY_POOL_H__