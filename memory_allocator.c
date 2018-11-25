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
                    (*((size_t *) current_block)) = ((*(size_t *) current_block) & ~(AVAILABLE_BIT)) +
                                                    ((*(size_t *) next_block) & ~(AVAILABLE_BIT)) + MANAGER_SIZE;
                    *((size_t *) current_block) |= AVAILABLE_BIT << 0;
                }
            }
        } else {
            /*set iterator to next block*/
            (size_t *) current_block =
                    ((size_t *) current_block) + ((*((size_t *) current_block)) & ~(AVAILABLE_BIT)) + MANAGER_SIZE;
        }
    }
    return NULL;
}


/* Return number of still allocated blocks */
size_t MemoryAllocator_free(MemoryAllocator *allocator, void *ptr) {


    void *current_block = allocator->m_memory_pool_ptr;
    size_t still_allocated_blocks = 0;
    size_t *end_of_memory_pool_ptr = (size_t *) current_block + allocator->m_memory_size;

    *((size_t *) ptr) |= AVAILABLE_BIT << 0;

    while (current_block != end_of_memory_pool_ptr) {
        if ((*((size_t *) current_block) & OCCUPIED_BIT) == OCCUPIED_BIT)
            still_allocated_blocks += 1;
        (size_t *) current_block =
                (size_t *) current_block + ((*((size_t *) current_block)) & ~(1)) + MANAGER_SIZE;
    }

    return still_allocated_blocks;
}


/* Return the size of largest free block */
size_t MemoryAllocator_optimize(MemoryAllocator *allocator) {

    void *current_block = allocator->m_memory_pool_ptr;
    size_t largest_free_block = ((*((size_t *) current_block)) & ~(1));
    size_t *end_of_memory_pool_ptr = (size_t *) current_block + allocator->m_memory_size;

    MemoryAllocator_allocate(allocator, allocator->m_memory_size);

    while (current_block != end_of_memory_pool_ptr) {

        if (((*((size_t *) current_block) & AVAILABLE_BIT) == AVAILABLE_BIT) && ((*((size_t *) current_block)) & ~(1)) > largest_free_block)
            largest_free_block = ((*((size_t *) current_block)) & ~(1));

        (size_t *) current_block =
                (size_t *) current_block + ((*((size_t *) current_block)) & ~(1)) + MANAGER_SIZE;
    }

    return largest_free_block;
}