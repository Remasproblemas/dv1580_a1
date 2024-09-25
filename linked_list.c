#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

  typedef struct Node {
    uint16_t data; // Stores the data as an unsigned 16-bit integer
    struct Node* next; // A pointer to the next node in the List
  } Node;


void list_init(Node** head){
  *head = NULL;
}

void list_insert(Node** head, int data){
  Node *new = NULL;

  *head = new;
  new->data = data;
}

void list_insert_after(Node* prev_node, int data);
void list_insert_before(Node** head, Node* next_node, int data);
void list_delete(Node** head, int data);
Node* list_search(Node** head, int data);
void list_display(Node** head);
void list_display_range(Node** head, Node* start_node, Node* end_node);
void list_cleanup(Node** head);

// int main(void){

//   Node *head = NULL;
//   list_init(&head);
//   list_insert(&head, 10);

//     return 0;
// }