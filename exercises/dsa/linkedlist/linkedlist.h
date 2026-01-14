// Include all dependencies required to run the functions defined in the header. This prevents the users of your API from having to find and import the dependencies themselves
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/**
 * Doubly linked list struct. Allocated on stack with nodes allocated in heap.
 */
struct linked_list {
    unsigned long long size;
    struct node* head;
    struct node* tail;
};

/**
 * Creates linked list struct. 
 */
struct linked_list ll_create(void);

/**
 * Frees each node of the linked list.
 */
void ll_free(struct linked_list* list);

/**
 * Adds data to head of linked list.
 */
void ll_add_first(struct linked_list *const list, const void* data);

/**
 * Adds data to tail of linked list.
 */
void ll_add_last(struct linked_list *const list, const void* data);

/**
 * Adds data at a given position in the linked list.
 */
void ll_add(struct linked_list *const list, const unsigned long long index, const void* data);

/**
 * Removes the node at the head of the linked list.
 */
const void* ll_remove_first(struct linked_list *const list);

/**
 * Removes the node at the tail of the linked list.
 */
const void* ll_remove_last(struct linked_list *const list);

/**
 * Removes the node at a given position in the linked list.
 */
const void* ll_remove(struct linked_list *const list, const unsigned long long index);

/**
 * Gets the data in the node at the given position.
 */
const void* ll_get(const struct linked_list *const list, const unsigned long long index);

/**
 * Updates the data in the node at the given position.
 */
void ll_set(const struct linked_list *const list, const unsigned long long index, const void* data);

/**
 * Determines whether or not the linked list is empty.
 */
bool ll_is_empty(const struct linked_list *const list);

/**
 * Determines whether or not a linked list contains the given data using a compare function.
 */
bool ll_contains(const struct linked_list *const list, const void* target, int (*compare)(const void*, const void*));

#endif