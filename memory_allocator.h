/*/
// Created by alian on 11/23/18.
/*/

#ifndef EXCELLENTEAM_ELLA_CPP_MEMORY_ALLOCATOR_WALL_ET_MEMORY_ALLOCATOR_H
#define EXCELLENTEAM_ELLA_CPP_MEMORY_ALLOCATOR_WALL_ET_MEMORY_ALLOCATOR_H

#include <stdio.h>


typedef struct MemoryAllocator MemoryAllocator;

MemoryAllocator *MemoryAllocator_init(void *memoryPool, size_t size);

void *MemoryAllocator_allocate(MemoryAllocator *allocator, size_t size);

/* Return number of still allocated blocks */
size_t MemoryAllocator_free(MemoryAllocator *allocator, void *ptr);

/* Return the size of largest free block */
size_t MemoryAllocator_optimize(MemoryAllocator *allocator);

#endif /*EXCELLENTEAM_ELLA_CPP_MEMORY_ALLOCATOR_WALL_ET_MEMORY_ALLOCATOR_H*/
