#include "memory_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>

  typedef struct Node {
    uint16_t data; // Stores the data as an unsigned 16-bit integer
    struct Node* next; // A pointer to the next node in the List
  } Node;

#define NODE_SIZE sizeof(Node)

//This function sets up the list and prepares it for operations.
void list_init(Node** head, size_t size) {
    mem_init(size * NODE_SIZE);
    if (!head) {
        *head = NULL;
    }
}

//Adds a new node with the specified data at end of the linked list. 
void list_insert(Node** head, uint16_t data) {
  if (*head == NULL) {
    Node *new_node = (Node*) mem_alloc(data); // ALLOCATE MEMORY FOR THE NEW NODE
    if (new_node == NULL) {
      printf("Failed to allocate memory for new Node\n");
      return;
    }

    printf("Memory allocation successfull\n");
    *head = new_node;
    new_node->data = data;
    new_node->next = NULL;
  } else {
  
    Node *current = *head;
    while (current->next != NULL){ // Find the last Node
      current = current->next;
    }
    
    Node *new_node = (Node*) mem_alloc(NODE_SIZE);
    if (new_node == NULL){
      printf("Failed to allocate memory for new Node\n");
      printf("I AM HERE\n");
      return;
    }

    new_node->data = data;
    new_node->next = NULL;
    current->next = new_node;
  }
}

// Inserts a new node with the specified data immediately after a given node.
void list_insert_after(Node* prev_node, uint16_t data){
  if (prev_node->next == NULL) {
    Node *new_node = (Node*) mem_alloc(NODE_SIZE); // ALLOCATE MEMORY FOR THE NEW NODE
    if (new_node == NULL) {
      printf("Failed to allocate memory for new Node\n");
      return;
    }
    prev_node->next = new_node;
    new_node->data = data;
    new_node->next = NULL;

  } else {
    Node *new_node = (Node*) mem_alloc(NODE_SIZE);
    if (new_node == NULL){
      printf("Failed to allocate memory for new Node\n");
      return;
    }
    new_node->data = data;
    new_node->next = prev_node->next;
    prev_node->next = new_node;
  }
}

// Inserts a new node with the specified data immediately before a given node in the list. 
void list_insert_before(Node** head, Node* next_node, uint16_t data){
  if (*head == next_node) {
    Node *new_node = (Node*) mem_alloc(NODE_SIZE); // ALLOCATE MEMORY FOR THE NEW NODE
    if (new_node == NULL) {
      printf("Failed to allocate memory for new Node\n");
      return;
    }
    new_node->data = data;
    new_node->next = next_node;
    *head = new_node;

  } else {

    Node *current = *head;
    while (current->next != next_node){ // Find next_node
      current = current->next;
    }

    Node *new_node = (Node*) mem_alloc(NODE_SIZE);
    if (new_node == NULL){
      printf("Failed to allocate memory for new Node\n");
      return;
    }
    new_node->data = data;
    new_node->next = next_node;
    current->next = new_node;
  }

}

// Removes a node with the specified data from the linked list.
void list_delete(Node** head, uint16_t data){  
  Node *current = *head;

  while (current->next->data != data){
    current = current->next;
  }

  Node* node_to_free = current->next;
  current->next = current->next->next;
  mem_free(node_to_free);
}

// Searches for a node with the specified data and returns a pointer to it.
Node* list_search(Node** head, uint16_t data){
  Node *current = *head;
  while (current->data != data){
    current = current->next;
  }
  return current;

}

// Prints all the elements in the linked list.
void list_display(Node** head){
    Node *current = *head;
    printf("[");

    while (current != NULL)
    {
        printf("%d", current->data);
        current = current->next;
        if (current != NULL){
          printf(", ");
        }
    }

    printf("]\n");
    return;
}

// Prints all elements of the list between two nodes (start_node and end_node).
void list_display_range(Node** head, Node* start_node, Node* end_node){
  Node *current = *head;
  printf("[");

  if(start_node != NULL){
    current = start_node;
  }

  if(end_node == NULL){
    while (current != NULL)
    {
        printf("%d", current->data);
        current = current->next;
        if (current != NULL){
          printf(", ");
        }
    }
  } else {
    while (current != end_node)
    {
        printf("%d", current->data);
        current = current->next;
        if (current != NULL){
          printf(", ");
        }
    }
    printf("%d", end_node->data);
  }

  printf("]\n");
  return;
}

//Will simply return the count of nodes.
int list_count_nodes(Node** head) {
  Node *current = *head;
  
  if(current == NULL){
    return 0;
  }
  
  int counter = 1;
  
  while (current->next != NULL){
    current = current->next;
    counter += 1;
  }

  return counter;
}

//Frees all the nodes in the linked list.
void list_cleanup(Node** head) {
  Node *current = *head;
  Node *next_node;

  while (current != NULL){
    next_node = current->next;
    mem_free(current);
    current = next_node;
  }
  *head = NULL;
  mem_deinit();
}

// int main(void){


//   printf("Size of Node %ld\n", NODE_SIZE);
//   Node *head = NULL;
//   list_init(&head, 32);
//   list_insert(&head, 10);
//   list_insert(&head, 20);
//   list_display(&head);
//   list_cleanup(&head);
//   list_display(&head);


//   return 0;
// }