#include <stdio.h>
#include <stdlib.h>

// Global pointer to the memory pool
void *memory_pool = NULL;

//MemoryBlock structure
typedef struct MemoryBlock {
    size_t size; //size in bytes
    int free; //1 for True, 0 for False
    struct MemoryBlock *next; //points to the next MemoryBlock
} MemoryBlock;

//Pointer to the head of the linked list of memory blocks
MemoryBlock *free_list = NULL;

//Macro, writing BLOCK_SIZE is the same as writing sizeof(MemoryBlock)
#define BLOCK_SIZE sizeof(MemoryBlock)

// Declare Functions
void mem_init(size_t size);
void* mem_alloc(size_t size);
void mem_free(void* block);
void* mem_resize(void* block, size_t size);
void mem_deinit();

int main(void){

    int i = 0;
    while(i < 100){
        mem_init(1024);
        
        i++;
    }
    
    return 0;
}

void mem_init(size_t size) {

    memory_pool = malloc(size);
    
    free_list = (MemoryBlock*) memory_pool; //Assign memory_pool to free_list and type cast it to a MemoryBlock
    free_list->size = size - BLOCK_SIZE;
    free_list->free = 1;
    free_list->next = NULL;
}
