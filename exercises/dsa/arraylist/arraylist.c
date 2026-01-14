#include <stdlib.h>
#include <stdbool.h>

#define STARTING_SIZE 5

struct array_list {
    unsigned long long size;
    unsigned long long max_index;
    // need double indirection because we want a pointer to a void pointer, not a void.
    void** elements;
};

struct array_list* create(void) {
    struct array_list list;
    list.size = 0;
    list.max_index = STARTING_SIZE - 1;
    list.elements = malloc(STARTING_SIZE * sizeof(void*));
    // Dangling pointer here
    return &list;
}

void add(struct array_list* list, void* value) {
    // Error
    if (list == NULL || list->elements == NULL) 
        return;
    if (list->size - 1 == list->max_index)     
        grow(list);
    // What if error during reallocation?
    list->elements[list->size - 1] = value;
    list->size++;
}

void* get(struct array_list* list, unsigned long long index) {
    if (list == NULL || list->elements == NULL) 
        return NULL;
    return list->elements[index];
}

void set(struct array_list* list, unsigned long long index, void* ptr) {
    // Error
    if (list == NULL || list->elements == NULL || index < 0 || index >= list->size)
        return;
    list->elements[index] = ptr;
}

bool is_empty(struct array_list* list) {
    // Does a null list count as an empty list even though it hasn't been initialized?
    // Argument for yes: technically a non-existent list is empty because a non-existing list cannot contain items. Moreover, it doesn't contain a pointer to any items
    // Argument for no: no space has been allocated. If no space has been allocated, can you really say you have a list? It doesn't exist in reality
    if (list == NULL)
        return false;
    return list->size == 0;
}

// better alternatives than returning -1?
// cannot return -1 because result is unsigned
unsigned long long size(struct array_list* list) {
    if (list == NULL)
        return -1;
    return list->size;
}

bool contains(struct array_list* list, void* target, int (*compare)(void*, void*)) {
    if (list == NULL || list->elements == NULL)
        return false;
    bool found = false;
    for (int i = 0; i < list->size && !found; i++) {
        found = compare(list->elements[i], target) == 0;
    }
    return found;
}

//
unsigned long long index_of(struct array_list* list, void* target, int (*compare)(void*, void*)) {
    // if (list == NULL)
}

// Error handling?
// option 1; return int
void grow(struct array_list* list) {
    unsigned long long new_max = (2 * list->size * sizeof(void*)) - 1;
    list->elements = realloc(list->elements, new_max + 1);
    list->max_index = new_max;
}

