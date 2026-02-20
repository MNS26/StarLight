#pragma once

#include "defines.h"
#include "includes.h"
#include "core/memory.h"

template <typename T>
struct Allocator {
  using value_type = T;
  Allocator() = default;

  template <typename U> 
  Allocator(const Allocator<U>&) {}

  template <typename U>
  struct rebind {
    using other = Allocator<U>;
  };

  T* allocate(std::size_t n) {
    if (n > std::numeric_limits<std::size_t>::max()/sizeof(T))
      throw std::bad_alloc();
    if (auto p = static_cast<T*>(SLallocate(n* sizeof(T), MEMORY_TAG_DARRAY)))
      return p;
    throw std::bad_alloc();
  }
  void deallocate(T* p, std::size_t n) noexcept {
    SLfree(p, n*sizeof(T), MEMORY_TAG_DARRAY);
  }
};

template <typename T, typename U>
bool operator==(const Allocator<T>&, const Allocator<U>&) { return true; }

template <typename T, typename U>
bool operator!=(const Allocator<T>&, const Allocator<U>&) { return false; }

template <typename T>
using DArray = std::vector<T, Allocator<T>>;
