/*#include <stddef.h>
#include <stdio.h>*/
#include "memory_allocator.h"
#include <stdlib.h>


struct MemoryAllocator {
    size_t m_memory_size;
    void *m_memory_pool_ptr;
};

MemoryAllocator *MemoryAllocator_init(void *memoryPool, size_t size) {
    MemoryAllocator *mem = malloc(sizeof(MemoryAllocator));
    mem->m_memory_pool_ptr = memoryPool;
    if (size % sizeof(size_t)) {
        mem->m_memory_size = size - size % sizeof(size_t);
    }
    *((size_t*)mem->m_memory_pool_ptr)=mem->m_memory_size;
    *((size_t*)mem->m_memory_pool_ptr)|= 1 << 0;
    return mem;
}
/*
void *MemoryAllocator_allocate(MemoryAllocator *allocator, size_t size) {
    size_t* iter_ptr = allocator->m_memory_pool_ptr;
    size_t i;
    for(i = 0; i<*allocator->m_memory_size; ++i){

    }
}*/
/*
MemoryAllocator *MemoryAllocator_init(void *memoryPool, size_t size);

void *MemoryAllocator_allocate(MemoryAllocator *allocator, size_t size);


size_t MemoryAllocator_free(MemoryAllocator *allocator, void *ptr);

size_t MemoryAllocator_optimize(MemoryAllocator *allocator);*/