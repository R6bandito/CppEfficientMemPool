#include <iostream>
#include "MemoryPool.h"

template<typename T>
MemoryPool<T>::MemoryPool() noexcept
 :freelist(nullptr),Object_Size(sizeof(value_type)),Object_Num(256),alignas_Num(8)
  { Block_Alloc(); }

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

template<typename T>
void MemoryPool<T>::allocNumSet(size_type __n) {
  this->Object_Num = __n;
}

template<typename T>
void MemoryPool<T>::alignasNumSet(size_type __n) {
  this->alignas_Num = __n;
}

template<typename T>
void MemoryPool<T>::Block_Alloc() {
  void* block_ptr = ::operator new(this->Object_Size * this->Object_Num, std::nothrow);
  if (!block_ptr) { }
  auto p_block_ptr = (static_cast<char *>(block_ptr) + padCaculate(block_ptr));  //内存对齐
  for(int i = 0; i < Object_Num; ++i) {
    void *slots = static_cast<char *>(p_block_ptr) + (Object_Size * i);
    _Slot *Node = static_cast<_Slot *>(slots);
    Node->next = this->freelist;
    this->freelist = Node;
  }
  this->Block_Container.push_back(reinterpret_cast<_Slot *>(p_block_ptr));
}

template <typename T>
inline typename MemoryPool<T>::size_type
MemoryPool<T>::padCaculate(void *Block_ptr) {
  bool isAligna = reinterpret_cast<uintptr_t>(Block_ptr) % alignas_Num;
  if (isAligna) 
    return (alignas_Num - isAligna);
  else 
    return 0;
}

template <typename T>
inline typename MemoryPool<T>::pointer
MemoryPool<T>::allocate(size_type /*__n*/, const void * /*__p*/) {
  bool Alloc_Flag = true;
  for(auto &it : Block_Container) {
    if (it != nullptr) {
      _Slot *Node = it;
      assert(Node != nullptr);
      if (Node != nullptr) {
        Alloc_Flag = false;  //不用再拓展内存块
        it = Node->next;
        return reinterpret_cast<pointer>(Node);
      }
      else { }
    }
  }
  if (Alloc_Flag) {
    Block_Alloc();
    return reinterpret_cast<pointer>(this->freelist);
  }

  return nullptr;
}
