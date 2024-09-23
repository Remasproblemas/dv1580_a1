#ifndef MEMORY_MANAGER_H  // Include guard start
#define MEMORY_MANAGER_H

#include <stddef.h>  // For size_t

// MemoryBlock structure (optional, only if needed externally)
typedef struct MemoryBlock {
    size_t size;  // Size in bytes
    int free;     // 1 if free, 0 if allocated
    struct MemoryBlock *next;  // Pointer to next memory block
} MemoryBlock;

// Function declarations
void mem_init(size_t size);
void* mem_alloc(size_t size);
void mem_free(void* block);
void* mem_resize(void* block, size_t size);
void mem_deinit();

#endif  // MEMORY_MANAGER_H  // Include guard end
