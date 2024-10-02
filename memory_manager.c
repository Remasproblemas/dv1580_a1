#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// MemoryBlock structure
typedef struct MemoryBlock {
    size_t size; // Size in bytes
    int free; //Indicates if the block is free, 1 for True, 0 for False
    struct MemoryBlock *next; // Pointer to the next MemoryBlock
} MemoryBlock;

// Keeps track of the metadata of the memory block.
#define BLOCK_SIZE sizeof(MemoryBlock)

// Pointer to the head of the linked list of memory blocks
MemoryBlock *global_block = NULL;

// Initializes the memory pool with the given size
void mem_init(size_t size) {
    if(size <= BLOCK_SIZE){
        // If the size smaller or equal to the metadata, exit
        return;
    }
    
    // Allocate the memory for the entire pool, multiply size with BLOCK_SIZE to ensure that there is enough space.
    global_block = (MemoryBlock*) malloc(size * BLOCK_SIZE);

    if(global_block == NULL){
        // If the allocation fails, exit
        return;
    }
    
    // Set the size for the global block, set it to be free and add a null pointer to indicate it is the last in the list (for now).
    global_block->size = size;
    global_block->free = 1;
    global_block->next = NULL;
}

// Helper Function for mem_alloc
MemoryBlock* find_free_block(size_t size) {
    MemoryBlock *current = global_block;

    // Go through each memory_block until a free block with enough space is found
    while (current) {      
        if (current->free == 1 && current->size >= size){
            return current; 
        }
        current = current->next;
    }
    // If no suitable block is found, return NULL
    return NULL;
}

// Allocates requested size to a memory_block and puts the remaining space into a new_block 
void* mem_alloc(size_t size){
    // Find a free memory block
    MemoryBlock *block = find_free_block(size);    
    
    // If requested size is 0, return pointer to the metadata.
    if(size == 0) {
        return (char*)block + BLOCK_SIZE; 
    }

    // If no block is found return NULL
    if(!block) {
        return NULL;
    }

    // Mark the block as allocated.
    block->free = 0;

    // Split the block if it's larger than needed
    if(block->size >= size + BLOCK_SIZE){
        MemoryBlock *new_block = (MemoryBlock*)((char*)block + BLOCK_SIZE + size);
    
        new_block->size = block->size - size; // Set size of the new block to be the remaining space
        new_block->free = 1;                // Mark it as free
        new_block->next = block->next;   // Link the new block to the next
        block->size = size;              // Adjust the current block size
        block->next = new_block;        // Have the current block point to the new one.
    }
    // Return a pointer to the usable memory (after the metadata)
    return (char*)block + BLOCK_SIZE;
}

// Helper function for mem_free
MemoryBlock* get_block_ptr(void *block) {
    return (MemoryBlock*) block - 1; //returns the pointer to the metadata of said block
}

// Function to free allocated blocks
void mem_free(void* block) {
    // If no block given, simply go back
    if (!block){
        return;
    }
    
    // Get the pointer for the metadata
    MemoryBlock *block_ptr = get_block_ptr(block);
    block_ptr -> free = 1; // Mark it as free

    // Merge with the next block if it's free
    if (block_ptr->next && block_ptr->next->free) {
        block_ptr->size += BLOCK_SIZE + block_ptr->next->size;
        block_ptr->next = block_ptr->next->next; // Remove the next block from the list
    }
}

// Resizes the given block to the requested size
void* mem_resize(void* block, size_t size){
    // If block is NULL, allocate a new block
    if (!block){
        block = mem_alloc(size);
    }

    // Get the block's metadata
    MemoryBlock *block_ptr = get_block_ptr(block);
    
    // If the block is already big enough, return the same block
    if(block_ptr->size >= size){
        return block;
    }
    
    // Allocate a new block of the requested size
    void *new_block_ptr = mem_alloc(size);

    // If allocation fails, retun NULL
    if(!new_block_ptr){
        return NULL;
    }

    // Copy the data from the old block to the new.
    memcpy(new_block_ptr, block, block_ptr->size);
    mem_free(block); // Free old block
    return new_block_ptr;
}

// Frees the entire memory pool and deinitializes the memory pool
void mem_deinit(){
    if(global_block != NULL){
        free(global_block); // Free the memory pool
        global_block = NULL; // The the "head" to NULL 
    }
}