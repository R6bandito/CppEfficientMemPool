#include <iostream>
#include "MemoryPool.h"

template<typename T>
MemoryPool<T>::MemoryPool() noexcept
 :freelist(nullptr),Object_Size(sizeof(value_type)),Object_Num(512) {}

template<typename T>
MemoryPool<T>::MemoryPool(MemoryPool &&_memory_pool) noexcept {
  this->freelist = _memory_pool.freelist;
  _memory_pool.freelist = nullptr; 
  std::swap(this->Block_Container,_memory_pool.Block_Container);

#if __cplusplus > 201402L
  this->Object_Num = std::exchange(_memory_pool.Object_Num,0);
  this->Object_Size = std::exchange(_memory_pool.Object_Size,0);
#else
  this->Object_Num = _memory_pool.Object_Num;
  this->Object_Size = _memory_pool.Object_Size;
#endif
}

template<typename T>
MemoryPool<T>&
MemoryPool<T>::operator=(MemoryPool &&_memory_pool) noexcept {
  if (this != &_memory_pool) {
    this->freelist = _memory_pool.freelist;
    _memory_pool.freelist = nullptr;
    std::swap(this->Block_Container,_memory_pool->Block_Container);
#if __cplusplus > 201402L
    this->Object_Num = std::exchange(_memory_pool.Object_Num,0);
    this->Object_Size = std::exchange(_memory_pool.Object_Size,0);
#else
    this->Object_Num = _memory_pool.Object_Num;
    this->Object_Size = _memory_pool.Object_Size;
#endif
  }
  else { }
}

template<typename T>
inline typename MemoryPool<T>::pointer
MemoryPool<T>::address(reference __X) const noexcept {
  return &__X;
}

template<typename T>
inline typename MemoryPool<T>::const_pointer
MemoryPool<T>::address(const_reference __X) const noexcept {
  return &__X;
}
