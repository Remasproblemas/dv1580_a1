#ifndef LINKED_LIST_H  // Include guard start
#define LINKED_LIST_H

#include <stdint.h>

typedef struct Node {
    uint16_t data; // Stores the data as an unsigned 16-bit integer
    struct Node* next; // A pointer to the next node in the List
  } Node;

void list_init(Node** head);
void list_insert(Node** head, int data);
void list_insert_after(Node* prev_node, int data);
void list_insert_before(Node** head, Node* next_node, int data);
void list_delete(Node** head, int data);
Node* list_search(Node** head, int data);
void list_display(Node** head);
void list_display_range(Node** head, Node* start_node, Node* end_node);
void list_cleanup(Node** head);

#endif  // LINKED_LIST_H // Include guard end
