#include <iostream>
#include "MemoryPool.h"

template<typename T>
MemoryPool<T>::MemoryPool() noexcept
 :freelist(nullptr),Object_Size(sizeof(value_type)),Object_Num(128),alignas_Num(8)
  { Block_Alloc(); }

template<typename T>
MemoryPool<T>::~MemoryPool() {
  for(auto it : Block_Container) {
    delete it;
  }
}

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
  size_type Block_size = this->Object_Size * this->Object_Num;
  void* block_ptr = ::operator new(Block_size, std::nothrow);
  char *_Block_tail = static_cast<char *>(block_ptr) + Block_size;  //内存块尾
  if (!block_ptr) { }
  size_type padPonter = padCaculate(block_ptr);
  auto _Block_head = (static_cast<char *>(block_ptr) + padPonter);  //内存对齐
  _Block_tail -= (alignas_Num - padPonter) % alignas_Num;
  size_type real_obj_Num = (_Block_tail - _Block_head) / Object_Size;

  assert(real_obj_Num > 0);

  for(int i = 0; i < real_obj_Num; ++i) {
    void *slots = static_cast<char *>(_Block_head) + (Object_Size * i);
    _Slot *Node = static_cast<_Slot *>(slots);
    Node->next = this->freelist;
    this->freelist = Node;
  }
  this->Block_Container.push_back(reinterpret_cast<char *>(_Block_head));
}

template <typename T>
inline typename MemoryPool<T>::size_type
MemoryPool<T>::padCaculate(void *Block_ptr) {
  size_type isAligna = reinterpret_cast<uintptr_t>(Block_ptr) % alignas_Num;
  if (isAligna) 
    return (alignas_Num - isAligna);
  else 
    return 0;
}

template <typename T>
typename MemoryPool<T>::pointer
MemoryPool<T>::allocate(size_type /*__n*/, const void * /*__p*/) {
  _Slot *Node = this->freelist;
  if (this->freelist->next == nullptr) {
    Block_Alloc();
  }
  else { this->freelist = this->freelist->next;}
  return reinterpret_cast<pointer>(Node);
}

template<typename T>
void MemoryPool<T>::deallocate(pointer __p, size_type /*__n*/) {
  if (__p != nullptr) {
    _Slot* Node = reinterpret_cast<_Slot *>(__p);
    Node->next = this->freelist;
    this->freelist = Node;
    /*std::cout << "deallocate. back to freelist" << std::endl;*/ 
  }
  else { }
}
