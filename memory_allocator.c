/*#include <stddef.h>
#include <stdio.h>*/
#include "memory_allocator.h"
#include <stdlib.h>


struct MemoryAllocator {
    size_t m_memory_size;
    void *m_memory_ptr;

};

MemoryAllocator *MemoryAllocator_init(void *memoryPool, size_t size) {
    MemoryAllocator *mem = malloc(sizeof(MemoryAllocator));
    mem->m_memory_ptr = memoryPool;
    if (size % sizeof(size_t)) {
        mem->m_memory_size = size - size % sizeof(size_t);
    }
    return mem;
}

/*
MemoryAllocator *MemoryAllocator_init(void *memoryPool, size_t size);

void *MemoryAllocator_allocate(MemoryAllocator *allocator, size_t size);


size_t MemoryAllocator_free(MemoryAllocator *allocator, void *ptr);

size_t MemoryAllocator_optimize(MemoryAllocator *allocator);*/