#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


//MemoryBlock structure
typedef struct MemoryBlock {
    size_t size; //size in bytes
    int free; //1 for True, 0 for False
    struct MemoryBlock *next; //points to the next MemoryBlock
} MemoryBlock;

//Keeps track of the metadata of the memory block.
#define BLOCK_SIZE sizeof(MemoryBlock)

//Pointer to the head of the linked list of memory blocks
MemoryBlock *global_block = NULL;

void mem_init(size_t size) {
    if(size <= BLOCK_SIZE){
        return;
    }
    
    global_block = (MemoryBlock*) malloc(size * BLOCK_SIZE);

    if(NULL == global_block){
        return;
    }
    
    global_block->size = size;
    global_block->free = 1;
    global_block->next = NULL;
}

// Helper Function for mem_alloc
MemoryBlock* find_free_block(size_t size) {
    MemoryBlock *current = global_block;

    while (current) {      
        if (current->free == 1 && current->size >= size){
            return current; 
        }
        current = current->next;
    }
    return NULL;
}

void print_memory_pool() {
    MemoryBlock* current_block = global_block;
    printf("Memory Pool:\n");
    while (current_block != NULL) {
        printf("Block at %p | Size: %zu | Is Empty: %d | Next: %p\n", 
        (void*)current_block,
        current_block->size,
        current_block->free,
        (void*)current_block->next);
        current_block = current_block->next;
    }
}

void* mem_alloc(size_t size){
    MemoryBlock *block = find_free_block(size);    
    if(size == 0) {
        return (char*)block + BLOCK_SIZE; 
    }

    if(!block) {
        return NULL;
    }

    block->free = 0;

    // Split the block if it's larger than needed
    if(block->size >= size + BLOCK_SIZE){
        MemoryBlock *new_block = (MemoryBlock*)((char*)block + BLOCK_SIZE + size); //(char*) is used so we move byte by byte instead of in chunks
        new_block->size = block->size - size;
        new_block->free = 1;
        new_block->next = block->next;   // Link the new block to the next
        block->size = size;              // Adjust the current block size
        block->next = new_block;
    }
    return (char*)block + BLOCK_SIZE;
}

// Helper function for mem_free
MemoryBlock* get_block_ptr(void *block) {
    return (MemoryBlock*) block - 1; //returns the pointer to the metadata of said block
}

void mem_free(void* block) {
    // If no block given, simply go back
    if (!block){
        return;
    }
    
    MemoryBlock *block_ptr = get_block_ptr(block);
    block_ptr -> free = 1;

    if (block_ptr->next && block_ptr->next->free) {
        block_ptr->size += BLOCK_SIZE + block_ptr->next->size;
        block_ptr->next = block_ptr->next->next; // Remove the next block from the list
    }
}

void* mem_resize(void* block, size_t size){
    if (!block){
        block = mem_alloc(size);
    }

    MemoryBlock *block_ptr = get_block_ptr(block);
    if(block_ptr->size >= size){
        return block;
    }
    
    void *new_block_ptr;
    new_block_ptr = mem_alloc(size);
    if(!new_block_ptr){
        return NULL;
    }

    memcpy(new_block_ptr, block, block_ptr->size);
    mem_free(block);
    return new_block_ptr;
}

void mem_deinit(){
    if(global_block != NULL){
        free(global_block);
        global_block = NULL;
    }
}


int main(void){

    mem_init(1024); // Initialize the memory pool with 1KB

    void *block0 = mem_alloc(0); // Edge case: zero allocation
    assert(block0 != NULL);      // Depending on handling, this could also be NULL

    void *block1 = mem_alloc(200); // Exactly remaining
    assert(block1 != NULL);
    assert(block0 == block1);
    

    mem_free(block0);
    mem_free(block1);
    mem_deinit();
    
    return 0;
}