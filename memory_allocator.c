/*#include <stddef.h>
#include <stdio.h>*/
#include "memory_allocator.h"
#include <stdlib.h>

#define AVAILABLE_BIT 0
#define MANAGER_SIZE sizeof(size_t)
#define BLOCK_SIZE sizeof(size_t)
#define OCCUPIED_BIT 1

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
    /*set manager with amount of available bits*/
    *((size_t *) mem->m_memory_pool_ptr) = mem->m_memory_size - MANAGER_SIZE;
    *((size_t *) mem->m_memory_pool_ptr) |= AVAILABLE_BIT << 0;
    return mem;
}

void *MemoryAllocator_allocate(MemoryAllocator *allocator, size_t size) {
    void *next_block;
    void *current_block = allocator->m_memory_pool_ptr;
    size_t index;
    size_t *end_of_memory_pool_ptr = (size_t *) current_block + allocator->m_memory_size;
    size_t aligned_size = size;

    while (aligned_size % sizeof(size_t))
        ++aligned_size;

    while (current_block != end_of_memory_pool_ptr) {
        if ((*((size_t *) current_block) & AVAILABLE_BIT) == AVAILABLE_BIT) {
            /*current block is free*/
            if ((*((size_t *) current_block)) & ~(AVAILABLE_BIT) >= aligned_size) {
                /*free big enough block found. set memory.*/
                if ((*((size_t *) current_block)) & ~(AVAILABLE_BIT) > aligned_size)){
                    /*block bigger then size. set rest as new block*/
                    (size_t *) next_block = ((size_t *) current_block) + aligned_size + MANAGER_SIZE;
                    (size_t *) next_block =
                            ((*((size_t *) current_block)) & ~(AVAILABLE_BIT)) - aligned_size - MANAGER_SIZE;
                    *((size_t *) next_block) |= AVAILABLE_BIT << 0;
                }
                *((size_t *) current_block) = aligned_size;
                *((size_t *) current_block) |= OCCUPIED_BIT << 0;
                return current_block;
            } else {
                (size_t *) next_block =
                        ((size_t *) current_block) + ((*(size_t *) current_block) & ~(AVAILABLE_BIT)) + MANAGER_SIZE;
                if ((*((size_t *) current_block)) & AVAILABLE_BIT) {
                    /*free block found. join with self*/
                    (*((size_t *) current_block)) = ((*(size_t *) current_block) & ~(AVAILABLE_BIT)) + ((*(size_t *) next_block) & ~(AVAILABLE_BIT)) + MANAGER_SIZE;
                    *((size_t *) current_block) |= AVAILABLE_BIT << 0;
                }
            }
        }
        *((size_t *) current_block) += ((size_t *) current_block) + ((*((size_t *) current_block)) & ~(AVAILABLE_BIT)) + MANAGER_SIZE;
    }
    return NULL;
}


/*
MemoryAllocator *MemoryAllocator_init(void *memoryPool, size_t size);

void *MemoryAllocator_allocate(MemoryAllocator *allocator, size_t size);


size_t MemoryAllocator_free(MemoryAllocator *allocator, void *ptr);

size_t MemoryAllocator_optimize(MemoryAllocator *allocator);*/